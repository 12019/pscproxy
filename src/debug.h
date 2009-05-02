#include "pscproxy-config.h"

#if defined(DEBUG) || defined(FORCE_DEBUG)
#include <cstdio>

#define pDebug(format, ...) printf("%s, %d: "format, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#define pqDebug(format, ...) printf(format, __VA_ARGS__)
#else // DEBUG
#define pDebug(format, ...)
#define pqDebug(format, ...)
#endif // DEBUG
