/**
 * @file file.h
 * @brief Defines file IO functions.
 * @if doc_files
 * @ingroup smug_platform
 * @endif
 */

/**
 * @addtogroup smug_platform
 * @{
 */

#ifndef SMUG_PLATFORM_FILE_H
#define SMUG_PLATFORM_FILE_H

struct _File;
typedef struct _File File;

#define SMUG_EOF (-1)
#define SMUG_SEEK_SET (1)
#define SMUG_SEEK_CUR (2)
#define SMUG_SEEK_END (3)

File* File_fopen(const char* filename, const char* mode);
int File_fclose(File* self);
size_t File_fread(File* self, void* ptr, size_t size, size_t count);
int File_fseek(File* self, long int offset, int origin);
long int File_ftell(File* self);

unsigned char* File_getBuffer(File* self);
void File_freeBuffer(File* self, unsigned char* buffer);
long int File_getLength(File* self);

#endif /* SMUG_PLATFORM_FILE_H */

/**@}*/
