#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <fstream>
#include <string>


class ImageCap
{
private:
    int fd; // A file descriptor to the video device
    int type;
    v4l2_buffer bufferinfo;
    char* buffer;
    int img_count;
public:
    ImageCap();
    ~ImageCap();
    int openCap();
    int takePicture(const std::string& img_dest);
};
