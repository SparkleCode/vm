#ifndef memory_h
#define memory_h

// given an array capacity, return a new capacity of the array after growing it
#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

// reallocate a pointer to an array to change the size of the array,
// bases new size of the array off sizeof(type)*capacity
#define GROW_ARRAY(previous, type, oldCount, count) \
    (type*)reallocate(previous, sizeof(type) * (oldCount), \
        sizeof(type) * (count))

// frees all memory allocated for an array
#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

// change the size of an array either malloc'ing or freeing
void* reallocate(void* previous, size_t oldSize, size_t newSize);

#endif // memory_h