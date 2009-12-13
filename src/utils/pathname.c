#include <stdlib.h>
#include "pathname.h"
#include "linkedlist.h"

static String* _separator = NULL;
static String* _dot = NULL;

static BOOL _invariant(PathName* p)
{
    return  (   NULL != p &&
                NULL != p->path &&
                (NULL != p->bareName ||
                NULL == p->extension)
            );
}

static String* _getDot()
{
    if (NULL == _dot)
    {
        _dot = String_new(".");
    }
    return _dot;
}

static String* _getSeparator()
{
    if (NULL == _separator)
    {
        _separator = String_new("/");
    }
    return _separator;
}

static BOOL _validPath(String* inputString)
{
    int state;
    int pos;
    char* str;
    String* inputCopy;
    String* validDriveLetters;
    String* invalidDirNameChars;

    validDriveLetters = String_new("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    invalidDirNameChars = String_new("/\\:*?\"<>|");

    state = 0;
    pos = 0;

    inputCopy = String_newCopy(inputString);
    String_toUppercase(inputCopy);
    str = inputCopy->mString;

    while (pos < inputString->mLength)
    {
        switch (state)
        {
        case 0: // Just starting out.
            if (str[pos] == '/' || str[pos] == '\\')
            {
                state = 1; //We have an absolute path on a unix system (accept backslash too, because we're lenient.).
            }
            else if (String_findChar(invalidDirNameChars, str[pos]) != -1)
            {
                state = 666; // Invalid path.
            }
            else if (String_findChar(validDriveLetters, str[pos]) != -1)
            {
                state = 2; // We have a windows drive letter, OR beginning of relative path.
            }
            else
            {
                state = 3; //Valid dirname character, but not letter. Beginning of relative path.
            }
            break;
        case 1: //Beginning of dir name.
            if (String_findChar(invalidDirNameChars, str[pos]) != -1)
            {
                state = 666; // Invalid path.
            }
            else
            {
                state = 3; //Valid dirname character.
            }
            break;
        case 2: //First char was letter. Either drive letter or beginning of dir name.
            if (str[pos] == ':' && (str[pos+1] == '/' || str[pos+1] == '\\'))
            {
                state = 1; pos++; // Absolute windows path.
            }
            else if (String_findChar(invalidDirNameChars, str[pos]) != -1)
            {
                state = 666; // Invalid path.
            }
            else
            {
                state = 3; // Relative path.
            }
            break;
        case 3: //Middle of dirname.
            if (str[pos] == '/' || str[pos] == '\\')
            {
                state = 1; // Found separator, new dirname.
            }
            else if (String_findChar(invalidDirNameChars, str[pos]) != -1)
            {
                state = 666; // Invalid path.
            }
            else
            {
                state = 3; // Name continues.
            }
            break;
        default:
            assert(FALSE);
            break;
        }
        pos++;
    }
    String_delete(inputCopy);
    String_delete(validDriveLetters);
    String_delete(invalidDirNameChars);
    return (state != 666);
}

PathName* PathName_new(String* string, BOOL isFile)
{
    PathName* newPn;
    int pos;
    String* pathWoRoot;
    String* separators;
    String* fileName;

    // Validate the path.
    if (!_validPath(string)) return NULL;

    separators = String_new("\\/");
    newPn = (PathName*)malloc(sizeof(PathName));

    // Check the root.
    pos = String_findChar(string, ':');
    if (1 == pos)
    {   // This is a Windows absolute path.
        newPn->root = String_newSubString(string, 0, 3);
        pos = 3;
    }
    else if (String_findChar(string, '/') == 0 || String_findChar(string, '\\') == 0)
    {   // This is a Unix absolute path.
        newPn->root = String_new("/");
        pos = 1;
    }
    else
    {   // This is a relative path.
        newPn->root = NULL;
        pos = 0;
    }

    // Parse the rest of the path.
    pathWoRoot = String_newSubString(string, pos, -1);
    newPn->path = String_splitMultiSep(pathWoRoot, separators);
    String_delete(pathWoRoot);

    if (isFile)
    {   // Get the file name.
        if (String_isEmpty((String*)newPn->path->last->item))
        {   // This was supposed to be a file, but the path ended in '/' or '\'
            String_delete(separators);
            PathName_delete(newPn);
            return NULL;
        }
        fileName = (String*)newPn->path->last->item;
        LinkedList_remove(newPn->path, newPn->path->last);
        // Find the extension, if there is one.
        pos = String_findCharReverse(fileName, '.');
        if (-1 == pos)
        {   // No extension (no '.' in file name).
            newPn->bareName = fileName;
            newPn->extension = NULL;
        }
        else
        {   // At least one '.' in file name. Separate out extension.
            newPn->bareName = String_newSubString(fileName, 0, pos);
            newPn->extension = String_newSubString(fileName, pos + 1, -1);
            String_delete(fileName);
        }
    }
    else
    {   // No file name.
        newPn->bareName = NULL;
        newPn->extension = NULL;
        if (String_isEmpty((String*)newPn->path->last->item))
        {   // The string ended in '/' or '\'. Last path element is empty, so remove it.
            String_delete((String*)newPn->path->last->item);
            LinkedList_remove(newPn->path, newPn->path->last);
        }
    }

    String_delete(separators);
    assert(_invariant(newPn));
    return newPn;
}

void PathName_delete(PathName* self)
{
    assert(_invariant(self));
    if (self->root)
    {
        String_delete(self->root);
    }
    if (self->bareName)
    {
        String_delete(self->bareName);
        if (self->extension)
        {
            String_delete(self->extension);
        }
    }
    LinkedList_deleteContents(self->path, String_delete);
    LinkedList_delete(self->path);
    free(self);
}

PathName* PathName_newCopy(PathName* self)
{
    PathName* newPn;

    assert(_invariant(self));

    newPn = (PathName*)malloc(sizeof(PathName));
    if (self->root)
    {
        newPn->root = String_newCopy(self->root);
    }
    if (self->bareName)
    {
        newPn->bareName = String_newCopy(self->bareName);
        if (self->extension)
        {
            newPn->extension = String_newCopy(self->extension);
        }
    }
    newPn->path = LinkedList_deepCopy(self->path, String_newCopy);

    assert(_invariant(newPn));
    return newPn;
}

void PathName_setSeparator(String* sep)
{
    if (NULL != _separator)
    {
        String_delete(_separator);
    }
    _separator = sep;
}

String* PathName_getFileName(PathName* self)
{
    String* ret;
    String* temp;

    assert(_invariant(self));

    if (NULL == self->bareName) return NULL;

    if (NULL == self->extension || String_isEmpty(self->extension))
    {
        ret = String_newCopy(self->bareName);
    }
    else
    {
        temp = String_newConcat(self->bareName, _getDot());
        ret = String_newConcat(temp, self->extension);
        String_delete(temp);
    }
    return ret;
}

String* PathName_getExtension(PathName* self)
{
    assert(_invariant(self));
    if (NULL == self->extension) return NULL;
    return String_newCopy(self->extension);
}

String* PathName_getBareName(PathName* self)
{
    assert(_invariant(self));
    if (NULL == self->bareName) return NULL;
    return String_newCopy(self->bareName);
}

String* PathName_getAsString(PathName* self)
{
    assert(_invariant(self));
    String* path;
    String* fullPath;

    path = String_newJoin(self->path, _getSeparator());
    if (self->root != NULL)
    {
        fullPath = String_newConcat(self->root, path);
        String_delete(path);
    }
    else
    {
        fullPath = path;
    }
    return fullPath;
}

BOOL PathName_isRelative(PathName* self)
{
    assert(_invariant(self));
    return (NULL == self->root);
}

BOOL PathName_isAbsolute(PathName* self)
{
    assert(_invariant(self));
    return (NULL != self->root);
}

BOOL PathName_isFile(PathName* self)
{
    assert(_invariant(self));
    return (NULL != self->bareName);
}

void PathName_addBase(PathName* self, PathName* base)
{
    assert(_invariant(self));
    assert(_invariant(base));
    assert(PathName_isRelative(self));

    assert(_invariant(self));
    assert(_invariant(base));

    LinkedList* basePath;

    if (base->root)
    {
        self->root = String_newCopy(base->root);
    }

    basePath = LinkedList_deepCopy(base->path, String_newCopy);
    LinkedList_concat(basePath, self->path);
    self->path = basePath;
}

PathName* PathName_newAddBase(PathName* self, PathName* base)
{
    PathName* newPath;
    newPath = PathName_newCopy(self);
    PathName_addBase(newPath, base);
    return newPath;
}
