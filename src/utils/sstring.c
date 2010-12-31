#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <smugstd.h>
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

static int _findAnyCharFrom(String* self, String* chars, int start)
{
    smug_assert(NULL != self);
    int i;

    i = max(start, 0);

    while (i < self->mLength)
    {
        if (String_findChar(chars, String_charAt(self, i)) != -1)
        {
            return i;
        }
        i++;
    }
    return -1;
}

static int _findAnyChar(String* self, String* chars)
{
    return _findAnyCharFrom(self, chars, 0);
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

void String_delete(String* self)
{
    smug_assert(NULL != self);
    free(self->mString);
    free(self);
}

void String_deleteVoid(void* self)
{
    String_delete((String*)self);
}

int String_length(String* self)
{
    smug_assert(NULL != self);
    return self->mLength;
}

BOOL String_isEmpty(String* self)
{
    smug_assert(NULL != self);
    return (self->mLength == 0);
}

void String_println(String* self)
{
    printf(self->mString);
}

char String_charAt(String* self, int pos)
{
    smug_assert(NULL != self);
    smug_assert(pos >= 0 && pos < self->mLength);
    return *(self->mString + pos);
}

char* String_asCstr(String* self)
{
    smug_assert(NULL != self);
    return self->mString;
}

String* String_newCopy(String* self)
{
    smug_assert(NULL != self);
    return String_new(self->mString);
}

void* String_newCopyVoid(void* self)
{
    return (void*)String_newCopy((String*)self);
}

char _charToUpper(char c)
{
    if (c >= 97 && c <= 122)
        return c - 32;
    else
        return c;
}

char _charToLower(char c)
{
    if (c >= 65 && c <= 90)
        return c + 32;
    else
        return c;
}

void String_toUppercase(String* self)
{
    int i;
    for (i = 0; i < self->mLength; i++)
    {
        self->mString[i] = _charToUpper(self->mString[i]);
    }
}

void String_toLowercase(String* self)
{
    int i;
    for (i = 0; i < self->mLength; i++)
    {
        self->mString[i] = _charToLower(self->mString[i]);
    }
}

String* String_newSubString(String* self, int start, int numChars)
{
    smug_assert(NULL != self);
    smug_assert(start >= 0);
    int num;
    int i;
    char* temp;

    num = numChars < 0 ? self->mLength - start : min(numChars, self->mLength - start);
    temp = malloc(sizeof(char) * (num + 1));
    temp[num] = '\0';

    for (i = 0; i < num; i++)
    {
        *(temp + i) = *(self->mString + start + i);
    }
    return _new(temp);
}

String* String_newConcat(String* first, String* second)
{
    smug_assert(NULL != first);
    smug_assert(NULL != second);
    char* temp;

    temp = malloc(sizeof(char) * (first->mLength + second->mLength + 1));
    strcpy(temp, first->mString);
    strcat(temp, second->mString);
    return _new(temp);
}

String* String_newReplace(String* self, String* find, String* replace)
{
    smug_assert(!"Unimplemented: String_newReplace.");
    return NULL;
}

String* String_newReplaceChar(String* self, char find, String* replace)
{
    smug_assert(NULL != self);
    smug_assert(NULL != replace);
    char* newString;
    int write;
    int read;
    int lastRead;
    int finalSize;

    write = 0;
    read = 0;
    lastRead = 0;
    finalSize = self->mLength + String_occurences(self, find) * (replace->mLength - 1) + 1;
    newString = malloc(sizeof(char) * finalSize);
    newString[finalSize - 1] = '\0';

    read = String_findChar(self, find);

    while (read != -1)
    {
        strncpy(newString + write, self->mString + lastRead, read - lastRead);
        write += read - lastRead;
        strncpy(newString + write, replace->mString, replace->mLength);
        write += replace->mLength;
        read++;
        lastRead = read;
        read = String_findCharFrom(self, find, read);
    }
    strncpy(newString + write, self->mString + lastRead, self->mLength - lastRead);
    smug_assert(write + (self->mLength - lastRead) == strlen(newString));
    return _new(newString);
}

void String_crop(String* self, int start, int numChars)
{
    smug_assert(NULL != self);
    String* temp;
    temp = String_newSubString(self, start, numChars);
    free(self->mString);
    self->mString = temp->mString;
    self->mLength = temp->mLength;
    free(temp);
}

BOOL String_equalAscii(String* first, String* second)
{
    smug_assert(NULL != first);
    smug_assert(NULL != second);
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
    String* firstUpper;
    String* secondUpper;
    BOOL ret;
    firstUpper = String_newCopy(first);
    secondUpper = String_newCopy(second);
    ret = String_equalAscii(firstUpper, secondUpper);
    String_delete(firstUpper);
    String_delete(secondUpper);
    return ret;
}

int String_occurences(String* self, char find)
{
    smug_assert(NULL != self);
    int i;
    int n;
    i = 0;
    n = 0;

    for (i = 0; i < self->mLength; i++)
    {
        if (String_charAt(self, i) == find)
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
    smug_assert(pos < self->mLength);
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
    smug_assert(NULL != self);
    int i;

    i = max(start, 0);

    while (i < self->mLength)
    {
        if (String_charAt(self, i) == c)
        {
            return i;
        }
        i++;
    }
    return -1;
}

int String_findCharReverseFrom(String* self, char c, int start)
{
    smug_assert(NULL != self);
    int i;

    i = min(start, self->mLength - 1);

    while (i >= 0)
    {
        if (String_charAt(self, i) == c)
        {
            return i;
        }
        i--;
    }
    return -1;
}

int String_findCharReverse(String* self, char c)
{
    return String_findCharReverseFrom(self, c, self->mLength - 1);
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
    int pos;
    int fromPos;
    LinkedList* strings;

    pos = _findAnyChar(self, separators);
    fromPos = 0;
    strings = LinkedList_new();

    while (pos != -1)
    {
        LinkedList_addLast(strings, (void*)String_newSubString(self, fromPos, pos - fromPos));
        fromPos = pos + 1;
        pos = _findAnyCharFrom(self, separators, fromPos);
    }
    LinkedList_addLast(strings, (void*)String_newSubString(self, fromPos, self->mLength - fromPos));
    return strings;
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
