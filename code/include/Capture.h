#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/v4l2-common.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <iostream>

class Capture
{
private:
    int fd;
    char* buffer;
    struct v4l2_buffer bufferinfo;
public:
    Capture(/* args */);
    ~Capture();
    void start();
    void takePicture(const char* dest);
    void stop();

};
