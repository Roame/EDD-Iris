#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>


using namespace std;

uint8_t *buffer;


static int xioctl(int fd, int request, void *arg){
    int r;
    do r = ioctl(fd, request, arg);
    while(-1==r && EINTR == errno);
    return r;
}

int main(){
    int fd = 10;

    cout << "0";

    fd = open("/dev/video0", O_RDWR);

    cout << "1";

    if(fd == -1){
        perror("Opening video device");
        return 1;
    }

    cout << "2";

    struct v4l2_capability caps = {0};
    if(-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps)){
        perror("Querying capabilities");
        return 1;
    }

    cout << "3";

    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 320;
    fmt.fmt.pix.height = 240;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if(1 == xioctl(fd, VIDIOC_S_FMT, &fmt)){
        perror("Setting pixel format");
        return 1;
    }

    cout << "4";

    struct v4l2_requestbuffers req = {0};

    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(-1 == xioctl(fd, VIDIOC_REQBUFS, &req)){
        perror("Requesting buffer");
        return 1;
    }

    cout << "5";

    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory - V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1  == xioctl(fd, VIDIOC_QUERYBUF, &buf)){
        perror("Querying Buffer");
        return 1;
    }
    buffer = (uint8_t*) mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

    if(-1 == xioctl(fd, VIDIOC_STREAMON, &buf.type))
    {
        perror("Start Capture");
        return 1;
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval tv = {0};
    tv.tv_sec = 2;
    int r = select(fd+1, &fds, NULL, NULL, &tv);
    if(-1 == r)
    {
        perror("Waiting for Frame");
        return 1;
    }

    if(-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
    {
        perror("Retrieving Frame");
        return 1;
    }

    std::cout << *buffer;
}