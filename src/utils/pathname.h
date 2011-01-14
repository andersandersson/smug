/** @file pathname.h
  * @brief Defines a pathname type and related functions.
  * @ingroup smug_utils
  */

/** @addtogroup smug_utils
  * @{
  */

#ifndef SMUG_UTILS_PATHNAME_H
#define SMUG_UTILS_PATHNAME_H

#include <common/common.h>
#include <utils/sstring.h>

/** The PathName type.
  */
typedef struct PathName {
	String* bareName;
	String* extension;
	LinkedList* path;
	String* root;
} PathName;

/** Create a path name from a string
  *
  * The path string supplied can be any valid path using '/' or '\' as separators.
  * If it is absolute, it should begin with either "X:\", "X:/" or "/". If the path does not
  * point to a file, it MAY end with a separator.
  *
  * @relatesalso PathName
  * @param string The input string. Should describe a valid path to a directory or file name.
  * @param isFile. Pass TRUE if the path points to a file, FALSE if it points to a directory.
  * @return NULL if the string could not be parsed as a path. Otherwise, a pointer to the created path object.
  */
PathName* PathName_new(String* string, BOOL isFile);

/** Delete a PathName object
  *
  * @relatesalso PathName
  * @param self The PathName to delete.
  */
void PathName_delete(PathName* self);

/** Copy a PathName object
  *
  * @relatesalso PathName
  * @param self The PathName to copy.
  * @return A deep copy of the provided PathName.
  */
PathName* PathName_newCopy(PathName* self);

/** Use a custom separator.
  *
  * Sets the separator string to use in subsequent calls to getAsString.
  * Does not affect any other operations. Affects all strings.
  *
  * @relatesalso PathName
  * @param sep The separator to use when rendering paths as strings.
  */
void PathName_setSeparator(String* sep);

/** Get the file name of this path.
  *
  * @relatesalso PathName
  * @param self The PathName.
  * @return The full filename as a string if the PathName points to a file. NULL otherwise.
  */
String* PathName_getFileName(PathName* self);

/** Get the extension of this path.
  *
  * @relatesalso PathName
  * @param self The PathName.
  * @return The extension as a string if the PathName points to a file with an extension. NULL otherwise.
  */
String* PathName_getExtension(PathName* self);

/** Get the bare file name of this path.
  *
  * @relatesalso PathName
  * @param self The PathName.
  * @return The bare filename (no extension) as a string if the PathName points to a file. NULL otherwise.
  */
String* PathName_getBareName(PathName* self);

/** Renders the path as a string.
  *
  * @relatesalso PathName
  * @param self The PathName.
  * @return A String object with the full path/filename of self, using the
  * current separator. The string will NOT include a trailing separator.
  */
String* PathName_getAsString(PathName* self);

/** Check if this is a relative path.
  *
  * @relatesalso PathName
  * @param self The PathName.
  * @return TRUE if this is a relative path, FALSE otherwise.
  */
BOOL PathName_isRelative(PathName* self);

/** Check if this is an absolute path.
  *
  * @relatesalso PathName
  * @param self The PathName.
  * @return TRUE if this is an absolute path, FALSE otherwise.
  */
BOOL PathName_isAbsolute(PathName* self);

/** Check if this path points to a file.
  *
  * @relatesalso PathName
  * @param self The PathName.
  * @return TRUE if this path points to a file, FALSE if it points to a directory.
  */
BOOL PathName_isFile(PathName* self);

/** Adds a base path to a relative path.
  *
  * @relatesalso PathName
  * @param self The relative path. Can be a file or a directory. Passing an asbolute path is an error. After the operation, this path will have been modifed to the path of (base + self).
  * @param base The base path. Can be absolute or relative. If it has a file name, it is ignored.
  */
void PathName_addBase(PathName* self, PathName* base);

/** Adds a base to a relative path.
  *
  * Same as above, but returns the result as a new path instead of modifying the first parameter.
  *
  * @relatesalso PathName
  * @param self The relative path. Can be a file or a directory. Passing an asbolute path is an error.
  * @param base The base path. Can be absolute or relative. If it has a file name, it is ignored.
  */
PathName* PathName_newAddBase(PathName* self, PathName* base);

#endif // PATHNAME_H

/**@}*/
