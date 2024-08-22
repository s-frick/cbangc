#ifndef ARRAY
#define ARRAY

#include <stdlib.h>
#include <string.h>

struct Array {
    size_t cap;
    size_t len;
    size_t stride;
    void* data;
};

enum {
    CAPACITY,
    LENGTH,
    STRIDE,
    DATA,
};

void *_array_create(size_t length, size_t stride);
void _array_destroy(void *arr);
void *_array_ensure_capacity(void *arr, size_t count);
void *_array_push(void *arr, void *value);

size_t _array_field_get(void *arr, size_t field);
void _array_field_set(void *arr, size_t field, size_t value);

#define DEFAULT_INIT 10
#define ARRAY_SCALE 2

#define array_create(type) \
    _array_create(DEFAULT_INIT, type)

#define array_destroy(arr) \
    _array_destroy(arr)
#define array_push(arr, x) arr = _array_push(arr, &x)
#define array_push_rval(arr, x) \
    do { \
	__auto_type temp = x; \
	arr = _array_push(arr, &temp); \
    } while(0)

#define array_ensure_capacity(arr, nElement) \
    _array_ensure_capacity(arr, nElement)

#define array_cap(arr) _array_field_get(arr, CAPACITY)
#define array_len(arr) _array_field_get(arr, LENGTH)
#define array_stride(arr) _array_field_get(arr, STRIDE)




#endif // !ARRAY
