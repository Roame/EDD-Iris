// #include "ImageCap.h"
#include <fcntl.h>
// #include <linux/ioctl.h>
#include <sys/ioctl.h>

#include <linux/v4l2-common.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <string.h>
#include <iostream>


int main() {
    int fd;
    fd = open("/dev/video0", O_RDWR);

    if(fd < 0){
        perror("Failed to open device");
        return 1;
    }

    v4l2_capability capability;
    if(-1 == ioctl(fd, VIDIOC_QUERYCAP, &capability)){
        perror("Could not query capabilities");
    }
    std::cout << std::hex << capability.capabilities << std::endl;
}

