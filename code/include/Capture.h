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
#include <chrono>

class Capture
{
private:
    int fd, numBuffers = 3, cBuffer = 0;
    char* buffers[3];
    struct v4l2_buffer bufferInfos[3];
    void initBuffers();
public:
    Capture(/* args */);
    ~Capture();
    void start();
    void takePicture(const char* dest);
    void stop();

};
