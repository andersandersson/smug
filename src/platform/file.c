#include <smugstd.h>
#include <common/common.h>
#include <common/log.h>
#include <platform/file.h>

struct _File
{
    FILE* mFile;
};

static File_new(FILE* file)
{
    File* newFile = (File*)malloc(sizeof(File));
    newFile->mFile = file;
    return newFile;
}

File* File_fopen(const char* filename, const char* mode)
{
    FILE* file = fopen(filename, mode);
    if (file)
    {
        return File_new(file);
    }
    return NULL;
}

int File_fclose(File* self)
{
    smug_assert(self != NULL);
    return fclose(self->mFile);
}

size_t File_fread(File* self, void* ptr, size_t size, size_t count)
{
    smug_assert(self != NULL);
    return fread(ptr, size, count, self->mFile);
}

int File_fseek(File* self, long int offset, int origin)
{
    smug_assert(self != NULL);
    switch (origin)
    {
        case SMUG_SEEK_SET:
            return fseek(self->mFile, offset, SEEK_SET);
        case SMUG_SEEK_CUR:
            return fseek(self->mFile, offset, SEEK_CUR);
        case SMUG_SEEK_END:
            return fseek(self->mFile, offset, SEEK_END);
        default:
            smug_assert(!"Invalid offset parameter to File_fseek!");
            return 1;
    }
}

long int File_ftell(File* self)
{
    smug_assert(self != NULL);
    return ftell(self->mFile);
}

unsigned char* File_getBuffer(File* self)
{
    smug_assert(self != NULL);
    long int length = File_getLength(self);
	unsigned char* buffer = (unsigned char*)malloc(length);
	if (File_fread(self, buffer, 1, length) != length)
	{
		ERROR("Did not read correct amount of bytes.");
		free(buffer);
        return NULL;
	}
    return buffer;
}

void File_freeBuffer(File* self, unsigned char* buffer)
{
    smug_assert(self != NULL);
    free(buffer);
}

long int File_getLength(File* self)
{
    smug_assert(self != NULL);
    long int oldPos = File_ftell(self);
	File_fseek(self, 0, SMUG_SEEK_END);
	long int len = File_ftell(self);
	File_fseek(file, oldPos, SMUG_SEEK_SET);
    return len;
}
