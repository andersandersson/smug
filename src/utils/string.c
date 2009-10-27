#include "string.h"

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

String* String_newSubString(String* this, int start, int numChars)
{
	assert(NULL != this);
	assert(start >= 0);
	int num;
	int i;
	char* temp;
	String* newStr;
	
	num = numChars < 0 ? this->mLength - start : min(numChars, this->mLength - start);
	temp = malloc(sizeof(char) * (num + 1));
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
	String* newStr;
	
	temp = malloc(sizeof(char) * (first->mLength + second->mLength + 1));
	strcpy(temp, first->mString);
	strcat(temp, second->mString);
	return _new(temp);
}

String* String_newReplace(String* this, String* find, String* replace)
{
	
}

String* String_newReplaceChar(String* this, char find, String* replace)
{
	assert(NULL != this);
	assert(NULL != replace);
	char* str;
	int n;
	int write;
	int read;
	int lastRead;
	int finalSize;
	
	write = 0;
	read = 0;
	lastRead = 0;
	finalSize = this->mLength + String_occurences(this, find) * (replace->mLength - 1) + 1;
	str = malloc(sizeof(char) * finalSize);
	str[finalSize - 1] = '\0';
	
	read = String_findChar(this, find);
	
	while (read != -1)
	{
		strncpy(str + write, this->mString + lastRead, read - lastRead);
		write += read - lastRead;
		strncpy(str + write, replace->mString, replace->mLength);
		write += replace->mLength;
		read++;
		lastRead = read;
		read = String_findCharFrom(this, find, read);
	}
	assert(write == strlen(str));
	return _new(str);
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
	assert(FALSE);
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
		i++;
	}
	return n;
}

int String_find(String* this, String* find)
{
	BOOL found;
	int pos;
	int i;

	found = FALSE;
	pos = -1;
	i = 0;

	while (!found && i < this->mLength - find->mLength + 1)
	{
		if (strncmp(this->mString + i, find->mString, find->mLength) == 0)
		{
			found = TRUE;
			pos = i;
			break;
		}
		i++;
	}
	return pos;
}

int String_findFrom(String* this, String* find, int start)
{
}

int String_findChar(String* this, char c)
{
	// assert(NULL != this);
	// BOOL found;
	// int pos;
	// int i;

	// found = FALSE;
	// pos = -1;
	// i = 0;

	// while (!found && i < this->mLength)
	// {
		// if (String_charAt(this, i) == c)
		// {
			// found = TRUE;
			// pos = i;
		// }
		// i++;
	// }
	// return pos;
	return String_findCharFrom(this, c, 0);
}

int String_findCharFrom(String* this, char c, int start)
{
	assert(NULL != this);
	BOOL found;
	int pos;
	int i;

	found = FALSE;
	pos = -1;
	i = start;

	while (!found && i < this->mLength)
	{
		if (String_charAt(this, i) == c)
		{
			found = TRUE;
			pos = i;
			break;
		}
		i++;
	}
	return pos;
}
