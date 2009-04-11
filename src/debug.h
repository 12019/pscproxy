#include "psproxy-config.h"

#ifdef DEBUG
#include <cstdio>

#define pDebug(format, ...) printf("%s, %d: "format, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
#else // DEBUG
#define pDebug(format, ...)
#endif // DEBUG
