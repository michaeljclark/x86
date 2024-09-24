#pragma once

#include "types.h"

static inline u16 be16(u16 v)
{
    union { u8 a[2]; u16 b; } u = {
        .a = { (u8)(v >> 8), (u8)(v) }
    };
    return u.b;
}

static inline u16 le16(u16 v)
{
    union { u8 a[2]; u16 b; } u = {
        .a = { (u8)(v), (u8)(v >> 8) }
    };
    return u.b;
}

static inline u32 be32(u32 v)
{
    union { u8 a[4]; u32 b; } u = {
        .a = { (u8)(v >> 24), (u8)(v >> 16), (u8)(v >> 8), (u8)(v) }
    };
    return u.b;
}

static inline u32 le32(u32 v)
{
    union { u8 a[4]; u32 b; } u = {
        .a = { (u8)(v), (u8)(v >> 8), (u8)(v >> 16), (u8)(v >> 24) }
    };
    return u.b;
}

static inline u64 be64(u64 v)
{
    union { u8 a[8]; u64 b; } u = {
        .a = { (u8)(v >> 56), (u8)(v >> 48), (u8)(v >> 40), (u8)(v >> 32),
               (u8)(v >> 24), (u8)(v >> 16), (u8)(v >> 8), (u8)(v) }
    };
    return u.b;
}

static inline u64 le64(u64 v)
{
    union { u8 a[8]; u64 b; } u = {
        .a = { (u8)(v), (u8)(v >> 8), (u8)(v >> 16), (u8)(v >> 24),
               (u8)(v >> 32), (u8)(v >> 40), (u8)(v >> 48), (u8)(v >> 56) }
    };
    return u.b;
}

#undef htobe16
#undef htole16
#undef be16toh
#undef le16toh

#undef htobe32
#undef htole32
#undef be32toh
#undef le32toh

#undef htobe64
#undef htole64
#undef be64toh
#undef le64toh

#define htobe16(x) be16(x)
#define htole16(x) le16(x)
#define be16toh(x) be16(x)
#define le16toh(x) le16(x)

#define htobe32(x) be32(x)
#define htole32(x) le32(x)
#define be32toh(x) be32(x)
#define le32toh(x) le32(x)

#define htobe64(x) be64(x)
#define htole64(x) le64(x)
#define be64toh(x) be64(x)
#define le64toh(x) le64(x)
