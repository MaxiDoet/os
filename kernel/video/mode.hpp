#ifndef _video_mode_hpp
#define _video_mode_hpp

#include <inttypes.h>

typedef enum videoColorDepth {
    VIDEO_COLOR_DEPTH_8 = 8,
    VIDEO_COLOR_DEPTH_16 = 16,
    VIDEO_COLOR_DEPTH_24 = 24,
} videoColorDepth;

typedef struct videoMode {
    uint16_t width;
    uint16_t height;
    videoColorDepth colorDepth;

    bool text;
} vgaVideoMode;

#endif