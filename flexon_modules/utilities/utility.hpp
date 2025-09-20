#ifndef __FLEXON_STRING_UTILITIES__
#define __FLEXON_STRING_UTILITIES__

#include <cstddef>
#include <cstdint>

namespace utility{
 void memcpy64(uint64_t *__src , uint64_t *__dst ,size_t size64);
 void memset64(void* __src , uint64_t ch , size_t size64);
 uint32_t fnv_1hash(uint32_t value);
}

#endif
