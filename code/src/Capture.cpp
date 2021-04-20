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
    // format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
    format.fmt.pix.field = V4L2_FIELD_NONE;

    xioctl("VIDIOC_S_FMT", fd, VIDIOC_S_FMT, &format);

    // Requesting buffers from video device
    struct v4l2_requestbuffers reqbuf;
    memset(&reqbuf, 0, sizeof(reqbuf));
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.count = numBuffers;

    xioctl("VIDEOC_REQBUFS", fd, VIDIOC_REQBUFS, &reqbuf);

    //Separate function to handle initializing and mapping all the buffers
    initBuffers();

    //Starting video stream
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl("VIDIOC_STREAMON", fd, VIDIOC_STREAMON, &type);
}

void Capture::takePicture(const char* dest){
    std::chrono::steady_clock::time_point beg;
    std::chrono::steady_clock::time_point end;
    /* The camera has two internal buffers. 
    Cycle 3 times to clear these and get a new image*/
    // Place VIDIOC_QBUF after file close to leverage buffer cycling
    xioctl("VIDIOC_QBUF", fd, VIDIOC_QBUF, &bufferInfos[cBuffer]);
    
    beg = chrono::steady_clock::now();
    xioctl("VIDIOC_DQBUF", fd, VIDIOC_DQBUF, &bufferInfos[cBuffer]);
    end = chrono::steady_clock::now();
    std::cout << "Dequeue time: " << std::chrono::duration_cast<chrono::milliseconds>(end-beg).count() << std::endl;
    
    // Preparing to read from the buffer:
    int remainingBufferSize = bufferInfos[cBuffer].bytesused;
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
        memcpy(outMemBlock, buffers[cBuffer]+bufPos, outMemBlockSize);
        outputFile.write(outMemBlock,outMemBlockSize);

        if(outMemBlockSize > remainingBufferSize)
            outMemBlockSize = remainingBufferSize;
        
        remainingBufferSize -= outMemBlockSize;

        delete outMemBlock;
    }

    // Close the image file
    outputFile.close();
    
    //Incrementing counter to cycle through buffers
    cBuffer++;
    if(cBuffer >= numBuffers) cBuffer = 0;
}

void Capture::stop(){
    close(fd);
}

void Capture::initBuffers(){
    for(int i=0; i < numBuffers; i++){
        //Querying buffer info
        struct v4l2_buffer qBuffer;
        memset(&qBuffer, 0, sizeof(qBuffer));
        qBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        qBuffer.memory = V4L2_MEMORY_MMAP;
        qBuffer.index = i;

        xioctl("VIDIOC_QUERYBUF", fd, VIDIOC_QUERYBUF, &qBuffer);

        //Mapping our buffer to devices buffer
        buffers[i] = (char*) mmap(NULL, qBuffer.length, 
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED,
                                fd, qBuffer.m.offset);

        //Constructing reference to buffer
        memset(&bufferInfos[i], 0, sizeof(bufferInfos[i]));
        bufferInfos[i].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        bufferInfos[i].memory = V4L2_MEMORY_MMAP;
        bufferInfos[i].index = i;
    }
}