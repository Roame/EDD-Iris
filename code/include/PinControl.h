#pragma once

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

class PinControl
{
private:
    int periodms = 10;
public:
    PinControl();
    ~PinControl(){}
    void stop();
    void setCycle(float dutyCycle);
};