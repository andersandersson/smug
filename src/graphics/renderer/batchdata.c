#include <stdlib.h>

#include <common/common.h>
#include <common/log.h>
#include <platform/opengl/opengl.h>

#include <graphics/renderer/batchdata.h>

static void freeData(BatchData* data)
{
    free(data->vertexData);
    free(data->colorData);
    free(data->textureData);
}

static void setSize(BatchData* data, unsigned int size)
{
    data->vertexData = malloc(sizeof(GLfloat) * size * 2);
    data->colorData = malloc(sizeof(GLfloat) * size * 4);
    data->textureData = malloc(sizeof(GLfloat) * size * 2);
}

static void copyData(BatchData* todata, BatchData* fromdata, unsigned int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        todata->vertexData[i + 0] = fromdata->vertexData[i + 0];
        todata->vertexData[i + 1] = fromdata->vertexData[i + 1];

        todata->colorData[i + 0] = fromdata->colorData[i + 0];
        todata->colorData[i + 1] = fromdata->colorData[i + 1];
        todata->colorData[i + 2] = fromdata->colorData[i + 2];
        todata->colorData[i + 3] = fromdata->colorData[i + 3];

        todata->textureData[i + 0] = fromdata->textureData[i + 0];
        todata->textureData[i + 1] = fromdata->textureData[i + 1];
    }

}

BatchData* BatchData_new(unsigned int initialSize)
{
    BatchData* ret = (BatchData*)malloc(sizeof(BatchData));
    ret->capacity = initialSize;
    setSize(ret, initialSize);

    return ret;
}

void BatchData_delete(void* data)
{
    BatchData* thedata = (BatchData*)data;
    freeData(thedata);
    free(thedata);
}

void BatchData_setCapacity(BatchData* data, unsigned int size)
{
    if (size <= data->capacity)
        return;

    freeData(data);
    setSize(data, size);

    data->capacity = size;
}

void BatchData_growCapacityTo(BatchData* data, unsigned int size)
{
    unsigned int newcap = data->capacity;

    while (size >= newcap)
    {
        newcap*=2;
    }

    BatchData_setCapacity(data, newcap);
}

void BatchData_expandTo(BatchData* data, unsigned int size)
{
    unsigned int oldcap = data->capacity;
    unsigned int newcap = data->capacity;

    while (size >= newcap)
    {
        newcap*=2;
    }

    BatchData* newdata = BatchData_new(newcap);

    copyData(newdata, data, oldcap);

    BatchData_delete(data);

    *data = *newdata;
}

unsigned int BatchData_getCapacity(BatchData* data)
{
    return data->capacity;
}
