/** @file string.h
  * @brief Defines a string type and related functions.
  */

#include "common/common.h"

#ifndef STRING_H
#define STRING_H

/** A struct for the string type.
  *
  * The first position of a String is zero. The length of a string is the number of characters in it.
  *
  */
typedef struct String {
	char* mString;
	int mLength;
} String;

/** Creates and returns a new string from the provided C string.
  *
  * @relatesalso String
  * @return A pointer to the string just created.
  */
String* String_new(char* aString);

/** Deletes a string.
  *
  * @relatesalso String
  */
void String_delete(String* this);

/** Gets the number of characters in a string.
  *
  * @relatesalso String
  * @return The number of characters in the string.
  */
int String_length(String* this);

BOOL String_isEmpty(String* this);

/** Gets a character from a string.
  *
  * Returns the character at position pos in the string.
  * pos must be non-negative and less that the length of the string.
  *
  * @relatesalso String
  * @param pos
  * @return The character in position pos.
  */
char String_charAt(String* this, int pos);

/** Gets the internal C string of the string.
  *
  * Returns a pointer to the C string held by the String. Do not free the
  * memory pointed to by it, or the String object will be broken.
  *
  * @relatesalso String
  * @return The C string representation of the string.
  */
char* String_asCstr(String* this);

/** Copies a string.
  *
  * @relatesalso String
  * @return A pointer to the resulting string.
  */
String* String_newCopy(String* this);

/** Finds a substring of a string.
  *
  * Creates a new string from the old one by taking numChars characters from it, starting in position start.
  * If there are less than numChars characters left in the string, the resulting string contains
  * all the characters in the string from start onwards.
  *
  * @relatesalso String
  * @param start The position where the substring begins. Must not be negative.
  * @param numChars The maximum number of characters to copy from the string.
  * @return A pointer to the resulting string.
  */
String* String_newSubString(String* this, int start, int numChars);

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
String* String_newReplace(String* this, String* find, String* replace);

/** Replace all occurences of a character within a string.
  *
  * Creates a new string by replacing all occurences of find in the string with replace.
  *
  * @relatesalso String
  * @param find The character to find.
  * @param replace The string to replace each occurence of find with.
  * @return A pointer to the resulting string.
  */
String* String_newReplaceChar(String* this, char find, String* replace);

/** Crops a string.
  *
  * Replaces the string with the substring starting in position start and continuing for maximum numChars characters.
  * 
  * @relatesalso String
  * @param start The position of the first character to keep.
  * @param numChars The maximum number of characters to keep.
  * @return A pointer to the resulting string.
  */
void String_crop(String* this, int start, int numChars);

/** Compares ascii values of two strings.
  *
  * @relatesalso String
  * @param first The first string.
  * @param second The second string.
  * @return TRUE if the strings are exactly equal, FALSE otherwise.
  */
BOOL String_equalAscii(String* first, String* second);

BOOL String_equalAlpha(String* first, String* second);

/** Counts the occurences of a character in a string.
  *
  * @relatesalso String
  * @param find The character to count.
  * @return The number of times the character find occurs in the string.
  */
int String_occurences(String* this, char find)

/** Finds the first occurence of a substring in a string.
  *
  * @relatesalso String
  * @param str The substring to find.
  * @return The position in the string of the first occurence of the substring str. -1 if str is not found within the string.
  */
int String_find(String* this, String* find);

/** Finds the next occurence of a substring in a string.
  *
  * @relatesalso String
  * @param str The substring to find.
  * @param start The position in which to start searching for the substring.
  * @return The position in the string of the first occurence of the substring str after position start. -1 if str is not found.
  */
int String_findFrom(String* this, String* find, int start);

/** Finds the first occurence of a character in a string.
  *
  * @relatesalso String
  * @param c The character to find.
  * @return The position in the string of the first occurence of the character c. -1 if c is not found.
  */
int String_findChar(String* this, char c);

/** Finds the next occurence of a character in a string.
  *
  * @relatesalso String
  * @param str The character to find.
  * @param start The position in which to start searching for the character.
  * @return The position in the string of the first occurence of the character c after position start. -1 if c is not found.
  */
int String_findCharFrom(String* this, char c, int start)

#endif // STRING_H
