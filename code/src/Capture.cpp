#include "Capture.h"

using namespace std;

void xioctl(const char* message, int fd, unsigned long request, void* arg){
    if(-1 == ioctl(fd, request, arg)){
        perror(message);
        exit(1);
    }
}

void assert(bool condition, const char* message){
    if(!condition){
        cout << message << endl;
        exit(1);
    }
}

Capture::Capture(){}
Capture::~Capture(){}

void Capture::start(){
    // Attempting to open video device
    fd = open("/dev/video0", O_RDWR);
    assert(fd > 0, "Couldn't open device");

    // Negotiating format
    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = 320;
    format.fmt.pix.height = 240;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
    format.fmt.pix.field = V4L2_FIELD_NONE;

    xioctl("VIDIOC_S_FMT", fd, VIDIOC_S_FMT, &format);

    // Requesting buffers from video device
    struct v4l2_requestbuffers reqbuf;
    memset(&reqbuf, 0, sizeof(reqbuf));
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.count = 1;

    xioctl("VIDEOC_REQBUFS", fd, VIDIOC_REQBUFS, &reqbuf);

    //Querying buffer info
    struct v4l2_buffer qBuffer;
    memset(&qBuffer, 0, sizeof(qBuffer));
    qBuffer.type = reqbuf.type;
    qBuffer.memory = V4L2_MEMORY_MMAP;
    qBuffer.index = 0;

    xioctl("VIDIOC_QUERYBUF", fd, VIDIOC_QUERYBUF, &qBuffer);

    //Mapping our buffer to devices buffer
    buffer = (char*) mmap(NULL, qBuffer.length, 
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd, qBuffer.m.offset);

    //Constructing reference to buffer
    memset(&bufferinfo, 0, sizeof(bufferinfo));
    bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;

    //Starting video stream
    int type = bufferinfo.type;
    xioctl("VIDIOC_STREAMON", fd, VIDIOC_STREAMON, &type);
}

void Capture::takePicture(const char* dest){
    /* The camera has two internal buffers. 
    Cycle 3 times to clear these and get a new image*/
    for(int i = 0; i < 1; i++){
        xioctl("VIDIOC_QBUF", fd, VIDIOC_QBUF, &bufferinfo);
        xioctl("VIDIOC_DQBUF", fd, VIDIOC_DQBUF, &bufferinfo);
    }

    // Preparing to read from the buffer:
    int remainingBufferSize = bufferinfo.bytesused;
    int bufPos = 0, outMemBlockSize = 0;
    char* outMemBlock = NULL;
    ofstream outputFile;
    outputFile.open(dest, ios::binary | ios::out);

    // Loop that reads chunks out of the buffer:
    while(remainingBufferSize > 0){
        bufPos += outMemBlockSize;

        outMemBlockSize = 320;
        outMemBlock = new char[sizeof(char) * outMemBlockSize];

        // Reading from memory and writing to image file
        memcpy(outMemBlock, buffer+bufPos, outMemBlockSize);
        outputFile.write(outMemBlock,outMemBlockSize);

        if(outMemBlockSize > remainingBufferSize)
            outMemBlockSize = remainingBufferSize;
        
        remainingBufferSize -= outMemBlockSize;

        delete outMemBlock;
    }
    // Close the image file
    outputFile.close();
}

void Capture::stop(){
    close(fd);
}