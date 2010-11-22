/** @file sstring.h
  * @brief Defines a string type and related functions.
  * @ingroup smug_utils
  */

/** @addtogroup smug_utils
  * @{
  */

#ifndef SMUG_UTILS_SSTRING_H
#define SMUG_UTILS_SSTRING_H

#include "common/common.h"
#include "linkedlist.h"

/** A struct for the string type.
  *
  * The first position of a String is zero. The length of a string is the number of characters in it.
  */
typedef struct String {
	char* mString;
	int mLength;
} String;

/** Creates and returns a new string from the provided C string.
  *
  * @relatesalso String
  * @param self The string.
  * @return A pointer to the string just created.
  */
String* String_new(char* aString);

/** Deletes a string.
  *
  * @relatesalso String
  * @param self The string.
  */
void String_delete(String* self);
void String_deleteVoid(void* self);

/** Gets the number of characters in a string.
  *
  * @relatesalso String
  * @param self The string.
  * @return The number of characters in the string.
  */
int String_length(String* self);

/** Checks if the string is the empty string.
  *
  * @relatesalso String
  * @param self The string.
  * @return TRUE if the string is empty, FALSE otherwise.
  */
BOOL String_isEmpty(String* self);

/** Prints the string to standard output.
  *
  * @relatesalso String
  * @param self The string.
  */
void String_println(String* self);

/** Gets a character from a string.
  *
  * Returns the character at position pos in the string.
  * pos must be non-negative and less that the length of the string.
  *
  * @relatesalso String
  * @param self The string.
  * @param pos The position.
  * @return The character in position pos.
  */
char String_charAt(String* self, int pos);

/** Gets the internal C string of the string.
  *
  * Returns a pointer to the C string held by the String. Do not free the
  * memory pointed to by it, or the String object will be broken.
  *
  * @relatesalso String
  * @param self The string.
  * @return The C string representation of the string.
  */
char* String_asCstr(String* self);

/** Deep-copies a string.
  *
  * @relatesalso String
  * @param self The string.
  * @return A pointer to the resulting string.
  */
String* String_newCopy(String* self);
void* String_newCopyVoid(void* self);

/** Transforms all alphabetic characters in the string to uppercase.
  *
  * @relatesalso String
  * @param self The string.
  */
void String_toUppercase(String* self);

/** Transforms all alphabetic characters in the string to lowercase.
  *
  * @relatesalso String
  * @param self The string.
  */
void String_toLowercase(String* self);

/** Finds a substring of a string.
  *
  * Creates a new string from the old one by taking numChars characters from it, starting in position start.
  * If there are less than numChars characters left in the string, the resulting string contains
  * all the characters in the string from start onwards.
  *
  * @relatesalso String
  * @param self The string.
  * @param start The position where the substring begins. Must not be negative.
  * @param numChars The maximum number of characters to copy from the string. If self is negative, copies the rest of the string.
  * @return A pointer to the resulting string.
  */
String* String_newSubString(String* self, int start, int numChars);

/** Concatenates two strings.
  *
  * Creates a new string by concatenating two strings together.
  *
  * @relatesalso String
  * @param first The first string.
  * @param second The second string.
  * @return A pointer to the resulting string.
  */
String* String_newConcat(String* first, String* second);

/** Replace all occurences of a substring within a string.
  *
  * Creates a new string by replacing all occurences of find in the string with replace.
  *
  * @relatesalso String
  * @param find The substring to find.
  * @param replace The string to replace each occurence of find with.
  * @return A pointer to the resulting string.
  */
String* String_newReplace(String* self, String* find, String* replace);

/** Replace all occurences of a character within a string.
  *
  * Creates a new string by replacing all occurences of find in the string with replace.
  *
  * @relatesalso String
  * @param self The string.
  * @param find The character to find.
  * @param replace The string to replace each occurence of find with.
  * @return A pointer to the resulting string.
  */
String* String_newReplaceChar(String* self, char find, String* replace);

/** Crops a string.
  *
  * Replaces the string with its substring starting in position start and continuing for maximum numChars characters.
  *
  * @relatesalso String
  * @param self The string.
  * @param start The position of the first character to keep.
  * @param numChars The maximum number of characters to keep.
  * @return A pointer to the resulting string.
  */
void String_crop(String* self, int start, int numChars);

/** Compares ascii values of two strings.
  *
  * @relatesalso String
  * @param first The first string to compare.
  * @param second The second string to compare.
  * @return TRUE if the strings are exactly equal, FALSE otherwise.
  */
BOOL String_equalAscii(String* first, String* second);

/** Compares two strings alphabetically, so A = a.
  *
  * @relatesalso String
  * @param first The first string to compare.
  * @param second The second string to compare.
  * @return TRUE if the strings are alphabetically equal, FALSE otherwise.
  */
BOOL String_equalAlpha(String* first, String* second);

/** Counts the occurences of a character in a string.
  *
  * @relatesalso String
  * @param self The string.
  * @param find The character to count.
  * @return The number of times the character find occurs in the string.
  */
int String_occurences(String* self, char find);

/** Counts the occurences of a substring in a string.
  *
  * Does not count overlapping occurences.
  *
  * @relatesalso String
  * @param self The string.
  * @param find The substring to look for.
  */
int String_occurencesStr(String* self, String* find);

/** Counts the occurences of a substring in a string.
  *
  * Counts all occurences, even if they overlap.
  *
  * @relatesalso String
  * @param self The string.
  * @param find The substring to look for.
  */
int String_occurencesStrOverlap(String* self, String* find);

/** Finds the first occurence of a substring in a string.
  *
  * @relatesalso String
  * @param self The string.
  * @param str The substring to find.
  * @return The position in the string of the first occurence of the substring str. -1 if str is not found within the string.
  */
int String_find(String* self, String* find);

/** Finds the next occurence of a substring in a string.
  *
  * @relatesalso String
  * @param self The string.
  * @param str The substring to find.
  * @param start The position in which to start searching for the substring.
  * @return The position in the string of the first occurence of the substring str after position start. -1 if str is not found.
  */
int String_findFrom(String* self, String* find, int start);

/** Finds the first occurence of a character in a string.
  *
  * @relatesalso String
  * @param self The string.
  * @param c The character to find.
  * @return The position in the string of the first occurence of the character c. -1 if c is not found.
  */
int String_findChar(String* self, char c);

/** Finds the next occurence of a character in a string.
  *
  * @relatesalso String
  * @param self The string.
  * @param str The character to find.
  * @param start The position in which to start searching for the character.
  * @return The position in the string of the first occurence of the character c after or in position start. -1 if c is not found.
  */
int String_findCharFrom(String* self, char c, int start);

/** Finds the last occurence of a character in a string.
  *
  * @relatesalso String
  * @param self The string.
  * @param c The character to find.
  * @return The position in the string of the last occurence of the character c. -1 if c is not found.
  */
int String_findCharReverse(String* self, char c);

/** Finds the next occurence of a character in a string, searching backwards.
  *
  * @relatesalso String
  * @param self The string.
  * @param str The character to find.
  * @param start The position in which to start searching for the character.
  * @return The position in the string of the last occurence of the character c before or in position start. -1 if c is not found.
  */
int String_findCharReverseFrom(String* self, char c, int start);

/** Split a string into a series of strings.
  *
  * The string is split at the separator character. The separator itself is not
  * included in the resulting strings. If the string starts or ends with a
  * separator, the list will start or end with an empty string, respectively.
  * Also, where two or more separator characters are found in sequence, empty
  * strings will occur in the list.
  *
  * @relatesalso String
  * @param self The string.
  * @param separator The character to separate substrings at.
  * @return A list of the resulting strings in the same order as in the string.
  */
LinkedList* String_split(String* self, char separator);

/** Split a string into a series of strings.
  *
  * Works like String_split, but treats all characters in the separators
  * parameter as separator characters.
  *
  * @relatesalso String
  * @param self The string.
  * @param separators A string of separator character.
  * @return A list of the resulting strings in the same order as in the string.
  */
LinkedList* String_splitMultiSep(String* self, String* separators);

/** Split a string into a series of strings.
  *
  * Works like String_split, but uses a whole string of characters as the
  * separator. The separator string is not included in the output strings.
  * Where two occurences of the separator string are overlapping, the first
  * occurence is used. (So "cababac" is split by "aba" into "c|bac".)
  *
  * @relatesalso String
  * @param self The string.
  * @param separator The string to separate substrings at.
  * @return A list of the resulting strings in the same order as in the string.
  */
LinkedList* String_splitStr(String* self, String* separator);

/** Joins a list of strings into a single string.
  *
  * Joins all the strings in the list in sequence, into a single string, putting
  * a copy of the separator string between each substring.
  *
  * @relatesalso String
  * @param strings The string.
  * @param separator The character to insert between strings.
  * @return The concatenation of all the strings in the list.
  */
String* String_newJoin(LinkedList* strings, String* separator);

#endif // SMUG_UTILS_SSTRING_H

/**@}*/
