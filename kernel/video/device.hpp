#ifndef _video_device_hpp
#define _video_device_hpp

#include <video/mode.hpp>

class VideoDevice {
    /* Attributes */
    char *name;
    videoMode mode;

    /* Basic functions */
    void *(*setMode(videoMode mode));
};

#endif