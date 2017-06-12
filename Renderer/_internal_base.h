#ifndef __INTERNAL_BASE_H__
#define __INTERNAL_BASE_H__

#define DISALLOW_EVIL_CONSTRUCTORS(name) \
    name(const name &); \
    name &operator=(const name &)

#endif // __INTERNAL_BASE_H__