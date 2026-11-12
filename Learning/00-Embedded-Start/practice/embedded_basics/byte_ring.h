#ifndef S_DEMO_EMBEDDED_BYTE_RING_H
#define S_DEMO_EMBEDDED_BYTE_RING_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BYTE_RING_OK,
    BYTE_RING_EMPTY,
    BYTE_RING_FULL,
    BYTE_RING_INVALID_ARGUMENT,
    BYTE_RING_NOT_IMPLEMENTED
} ByteRingStatus;

typedef struct {
    uint8_t* storage;
    size_t capacity;
    size_t read_index;
    size_t size;
} ByteRing;

ByteRingStatus byte_ring_init(ByteRing* ring, uint8_t* storage, size_t capacity);
ByteRingStatus byte_ring_push(ByteRing* ring, uint8_t value);
ByteRingStatus byte_ring_pop(ByteRing* ring, uint8_t* value);

#ifdef __cplusplus
}
#endif

#endif
