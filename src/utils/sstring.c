#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sstring.h"

// Creates a String object from the C string. The C string is NOT copied.
String* _new(char* theString)
{
    String* newStr;
    newStr = malloc(sizeof(String));

    newStr->mLength = strlen(theString);
    newStr->mString = theString;

    return newStr;
}

String* String_new(char* aString)
{
    String* new_string;
    new_string = malloc(sizeof(String));

    // Determine string length;
    new_string->mLength = strlen(aString);

    // Copy the C string to the string object
    new_string->mString = malloc(sizeof(char) * (new_string->mLength + 1));
    strcpy(new_string->mString, aString);

    return new_string;
}

void String_delete(String* this)
{
    assert(NULL != this);
    free(this->mString);
    free(this);
}

void String_deleteVoid(void* self)
{
    String_delete((String*)self);
}

int String_length(String* this)
{
    assert(NULL != this);
    return this->mLength;
}

BOOL String_isEmpty(String* this)
{
    assert(NULL != this);
    return (this->mLength == 0);
}

void String_println(String* self)
{
    printf(self->mString);
}

char String_charAt(String* this, int pos)
{
    assert(NULL != this);
    assert(pos >= 0 && pos < this->mLength);
    return *(this->mString + pos);
}

char* String_asCstr(String* this)
{
    assert(NULL != this);
    return this->mString;
}

String* String_newCopy(String* this)
{
    assert(NULL != this);
    return String_new(this->mString);
}

void* String_newCopyVoid(void* self)
{
    return (void*)String_newCopy((String*)self);
}

void String_toUppercase(String* self)
{
    assert(!"Unimplemented function.");
}

void String_toLowercase(String* self)
{
    assert(!"Unimplemented function.");
}

String* String_newSubString(String* this, int start, int numChars)
{
    assert(NULL != this);
    assert(start >= 0);
    int num;
    int i;
    char* temp;

    num = numChars < 0 ? this->mLength - start : min(numChars, this->mLength - start);
    temp = malloc(sizeof(char) * (num + 1));
    temp[num] = '\0';

    for (i = 0; i < num; i++)
    {
        *(temp + i) = *(this->mString + start + i);
    }
    return _new(temp);
}

String* String_newConcat(String* first, String* second)
{
    assert(NULL != first);
    assert(NULL != second);
    char* temp;

    temp = malloc(sizeof(char) * (first->mLength + second->mLength + 1));
    strcpy(temp, first->mString);
    strcat(temp, second->mString);
    return _new(temp);
}

String* String_newReplace(String* this, String* find, String* replace)
{
    assert(!"Unimplemented: String_newReplace.");
    return NULL;
}

String* String_newReplaceChar(String* this, char find, String* replace)
{
    assert(NULL != this);
    assert(NULL != replace);
    char* newString;
    int write;
    int read;
    int lastRead;
    int finalSize;

    write = 0;
    read = 0;
    lastRead = 0;
    finalSize = this->mLength + String_occurences(this, find) * (replace->mLength - 1) + 1;
    newString = malloc(sizeof(char) * finalSize);
    newString[finalSize - 1] = '\0';

    read = String_findChar(this, find);

    while (read != -1)
    {
        strncpy(newString + write, this->mString + lastRead, read - lastRead);
        write += read - lastRead;
        strncpy(newString + write, replace->mString, replace->mLength);
        write += replace->mLength;
        read++;
        lastRead = read;
        read = String_findCharFrom(this, find, read);
    }
    strncpy(newString + write, this->mString + lastRead, this->mLength - lastRead);
    assert(write + (this->mLength - lastRead) == strlen(newString));
    return _new(newString);
}

void String_crop(String* this, int start, int numChars)
{
    assert(NULL != this);
    String* temp;
    temp = String_newSubString(this, start, numChars);
    free(this->mString);
    this->mString = temp->mString;
    this->mLength = temp->mLength;
    free(temp);
}

BOOL String_equalAscii(String* first, String* second)
{
    assert(NULL != first);
    assert(NULL != second);
    int i;
    BOOL fail;

    i = 0;
    fail = FALSE;

    if (first->mLength != second->mLength)
        return FALSE;

    while (i < first->mLength)
    {
        if (String_charAt(first, i) != String_charAt(second, i))
        {
            fail = TRUE;
            break;
        }
        i++;
    }
    return !fail;
}

BOOL String_equalAlpha(String* first, String* second)
{
    assert(!"Unimplemented function.");
    return FALSE;
}

int String_occurences(String* this, char find)
{
    assert(NULL != this);
    int i;
    int n;
    i = 0;
    n = 0;

    for (i = 0; i < this->mLength; i++)
    {
        if (String_charAt(this, i) == find)
        {
            n++;
        }
    }
    return n;
}

static int _occurencesStr(String* self, String* find, int skipNr)
{
    int i;
    int counter;
    i = String_find(self, find);
    while (-1 != i)
    {
        counter++;
        i = String_findFrom(self, find, i + skipNr);
    }
    return counter;
}

int String_occurencesStr(String* self, String* find)
{
    return _occurencesStr(self, find, find->mLength);
}

int String_occurencesStrOverlap(String* self, String* find)
{
    return _occurencesStr(self, find, 1);
}

int String_find(String* self, String* find)
{
    return String_findFrom(self, find, 0);
}

BOOL _compSkewed(String* self, int pos, String* find)
{
    assert(pos < self->mLength);
    if (find->mLength > self->mLength - pos) return FALSE;
    return 0 == strncmp(self->mString + pos, find->mString, self->mLength);
}

int String_findFrom(String* self, String* find, int start)
{
    int pos;
    int i;

    pos = -1;
    i = start;

    while (i <= self->mLength - find->mLength)
    {
        if (_compSkewed(self, i, find))
        {
            pos = i;
            break;
        }
        i++;
    }
    return pos;
}

int String_findChar(String* self, char c)
{
    return String_findCharFrom(self, c, 0);
}

int String_findCharFrom(String* self, char c, int start)
{
    assert(NULL != self);
    BOOL found;
    int pos;
    int i;

    found = FALSE;
    pos = -1;
    i = start;

    while (!found && i < self->mLength)
    {
        if (String_charAt(self, i) == c)
        {
            found = TRUE;
            pos = i;
            break;
        }
        i++;
    }
    return pos;
}

int String_findCharReverse(String* self, char c)
{
    assert(!"Unimplemented function.");
    return -1;
}

LinkedList* String_split(String* self, char separator)
{
    int pos;
    int fromPos;
    LinkedList* strings;

    pos = String_findChar(self, separator);
    fromPos = 0;
    strings = LinkedList_new();

    while (pos != -1)
    {
        LinkedList_addLast(strings, (void*)String_newSubString(self, fromPos, pos - fromPos));
        fromPos = pos + 1;
        pos = String_findCharFrom(self, separator, fromPos);
    }
    LinkedList_addLast(strings, (void*)String_newSubString(self, fromPos, self->mLength - fromPos));
    return strings;
}

LinkedList* String_splitMultiSep(String* self, String* separators)
{
    assert(!"Unimplemented function.");
    return NULL;
}

LinkedList* String_splitStr(String* self, String* separator)
{
    int pos;
    int fromPos;
    LinkedList* strings;

    pos = String_find(self, separator);
    fromPos = 0;
    strings = LinkedList_new();

    while (pos != -1)
    {
        LinkedList_addLast(strings, (void*)String_newSubString(self, fromPos, pos - fromPos));
        fromPos = pos + separator->mLength;
        pos = String_findFrom(self, separator, fromPos);
    }
    return strings;
}

String* String_newJoin(LinkedList* strings, String* separator)
{
    Node* iter;
    iter = strings->first;
    String* str;
    String* temp;

    str = String_new("");

    while (NULL != iter)
    {
        temp = String_newConcat(str, iter->item);
        String_delete(str);
        str = temp;
        iter = iter->next;
        if (NULL != iter)
        {
            temp = String_newConcat(str, separator);
            String_delete(str);
            str = temp;
        }
    }
    return str;
}
