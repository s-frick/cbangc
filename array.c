#include "array.h"
#include <stdlib.h>
#include <string.h>

void *_array_create(size_t length, size_t stride) {
    size_t header_size = DATA * sizeof(size_t); 
    size_t data_size = stride * length; 
    size_t *array  = (size_t *) malloc(header_size + data_size); 
    array[CAPACITY] = length; 
    array[LENGTH] = 0; 
    array[STRIDE] = stride; 
    return (void *) (array + DATA);
}

void _array_destroy(void *arr) {
    void *header = arr - ( DATA * sizeof(size_t) );
    free(header);
}
size_t _array_field_get(void *arr, size_t field) {
    return ((size_t *)(arr) - DATA)[field];
}
void _array_field_set(void *arr, size_t field, size_t value)
{
    ((size_t *)(arr) - DATA)[field] = value;
}

void *_array_ensure_capacity(void *arr, size_t count) {
     if (array_len(arr) + count >= array_cap(arr))  {
        size_t size = ( array_len(arr) + count ) > array_len(arr) * ARRAY_SCALE ? ( array_len(arr) + count ) : array_len(arr) * ARRAY_SCALE;

        void *tmp = _array_create(size, array_stride(arr));
        memcpy(tmp, arr, size * array_stride(arr));
        _array_field_set(tmp, LENGTH, array_len(arr));
        _array_destroy(arr);
        return tmp;
    }
    return arr;
}

void *_array_push(void *arr, void *value) {
    arr = array_ensure_capacity(arr, 1);
    memcpy(arr + array_len(arr) * array_stride(arr), value, array_stride(arr));
    _array_field_set(arr, LENGTH, array_len(arr)+ 1);
    return arr;
}
