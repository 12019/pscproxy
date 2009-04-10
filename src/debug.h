#include "pcproxy-config.h"

#warning 1
#ifdef DEBUG
#warning 2
#include <cstdio>

#define pDebug(format, ...) printf("%s, %d: "format, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#else // DEBUG
#define pDebug(format, ...)
#endif // DEBUG
#warning 3
