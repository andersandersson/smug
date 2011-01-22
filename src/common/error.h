#ifndef SMUG_COMMON_ERROR_H
#define SMUG_COMMON_ERROR_H

#include <common/common.h>
#include <common/log.h>

#define smug_error(cond, msg, ...) do { if (!(cond)) { ERROR(msg, ##__VA_ARGS__); } } while (0)

#endif /* SMUG_COMMON_ERROR_H */
