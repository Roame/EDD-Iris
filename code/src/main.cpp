// #include "ImageCap.h"
#include <fcntl.h>
// #include <linux/ioctl.h>
#include <sys/ioctl.h>

#include <linux/v4l2-common.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/mman.h>
#include <errno.h>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>

using namespace std;

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
    std::cout << "Webcam capabilities: " << std::hex << capability.capabilities << std::endl;

    struct v4l2_requestbuffers reqbuf;

    memset(&reqbuf, 0, sizeof(reqbuf));
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.count = 1;
    
    if(-1 == ioctl(fd, VIDIOC_REQBUFS, &reqbuf)){
        if(errno == EINVAL)
            printf("Video capturing or mmap-streaming is not supported \\n");
        else
            perror("VIDIOC_REQBUFS");
        return 1;
    }
    
    struct v4l2_buffer qBuffer;
    memset(&qBuffer, 0, sizeof(qBuffer));
    qBuffer.type = reqbuf.type;
    qBuffer.memory = V4L2_MEMORY_MMAP;
    qBuffer.index = 0;

    if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &qBuffer)){
        perror("VIDIOC_QUERYBUF");
        return 1;
    }

    char* buffer = (char*) mmap(NULL, qBuffer.length, 
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd, qBuffer.m.offset);

    struct v4l2_buffer bufferinfo;
    memset(&bufferinfo, 0, sizeof(bufferinfo));
    bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;

    int type = bufferinfo.type;
    if(-1 == ioctl(fd, VIDIOC_STREAMON, &type)){
        perror("VIDIOC_STREAMON");
        return 1;
    }

    //Read Loop:
    for(int i = 0; i < 2; i ++){
        cout << "Starting image " << i << endl;
        memset(&bufferinfo, 0, sizeof(bufferinfo));
        bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufferinfo.memory = V4L2_MEMORY_MMAP;
        bufferinfo.index = 0;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &bufferinfo)){
            perror("VIDIOC_QBUF");
            return 1;
        }
        if(-1 == ioctl(fd, VIDIOC_DQBUF, &bufferinfo)){
            perror("VIDIOC_DQBUF");
            return 1;
        }

        stringstream sstm;
        sstm << "test" << i << ".jpeg";
        ofstream output;
        output.open(sstm.str().c_str(), ios::binary | ios::app);
        
        int remainingBufferSize = bufferinfo.bytesused;
        int bufPos = 0, outMemBlockSize = 0;
        char* outMemBlock = NULL;
        int itr = 0;
        while(remainingBufferSize > 0){
            bufPos += outMemBlockSize;

            outMemBlockSize = 320;
            outMemBlock = new char[sizeof(char) * outMemBlockSize];

            //Reading from memory and writing to image file
            memcpy(outMemBlock, buffer+bufPos, outMemBlockSize);
            output.write(outMemBlock,outMemBlockSize);

            if(outMemBlockSize > remainingBufferSize)
                outMemBlockSize = remainingBufferSize;
            
            remainingBufferSize -= outMemBlockSize;

            delete outMemBlock;
        }
        output.close();
        cout << "Finished" << endl;
        usleep(5 * 1000000);
    }

    if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &type)){
        perror("VIDIOC_STREAMOFF");
        return 1;
    }

    close(fd);

    return 0;
}

