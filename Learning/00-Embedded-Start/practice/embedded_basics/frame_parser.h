#ifndef S_DEMO_EMBEDDED_FRAME_PARSER_H
#define S_DEMO_EMBEDDED_FRAME_PARSER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { FRAME_MAX_PAYLOAD_SIZE = 32 };

typedef enum {
    FRAME_OK,
    FRAME_INCOMPLETE,
    FRAME_INVALID_LENGTH,
    FRAME_INVALID_ARGUMENT,
    FRAME_NOT_IMPLEMENTED
} FrameStatus;

typedef struct {
    const uint8_t* payload;
    size_t size;
} FrameView;

FrameStatus parse_frame(const uint8_t* input, size_t input_size, FrameView* frame,
                        size_t* consumed);

#ifdef __cplusplus
}
#endif

#endif
