/** @file common.h
  * @brief Header file for common declarations
  * @ingroup smug_common
  */

/** @defgroup smug_common Common
  * @{
  */

#ifndef SMUG_COMMON_COMMON_H
#define SMUG_COMMON_COMMON_H

#ifdef HAS_CONFIG_H
    #include <config.h>
#endif

#ifndef SMUGEXPORT
    #define SMUGEXPORT
#endif /* SMUGEXPORT */

typedef unsigned char BOOL;
#define TRUE  1
#define FALSE 0

#ifndef NULL
    #define NULL 0
#endif

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#undef assert
#ifndef DISABLE_ASSERTS
    void Smug_assert(BOOL result, char* expression, char* file, int line);
    #define assert(x) Smug_assert(x, #x, __FILE__, __LINE__)
#else
    #define assert(x) x
#endif

void swap_float(float* left, float* right);

typedef double TIME;

#ifdef SMUG_GLES
    // Needed in OpenGL ES
    long floatToFixed(float value);
    #define FIXED(value) floatToFixed(value)
#endif /* SMUG_GLES */

#endif // SMUG_COMMON_COMMON_H

/**@}*/
