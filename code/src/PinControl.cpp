#include "PinControl.h"


void test(bool condition, const char *message){
    if(!condition){
        perror(message);
        exit(1);
    }
}

PinControl::PinControl(){
    int fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    test(fd != 1, "Could not open /sys/class/pwm/pwmchip0/export");
    write(fd, "0", 1); // No assertion here to bypass exporting errors
    close(fd);

    std::string period_str = std::to_string((int)(periodms*1e+6));

    fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
    test(fd != 1, "Could not open /sys/class/pwm/pwmchip0/pwm0/period");
    test(write(fd, period_str.c_str(), period_str.size()) == period_str.size(), "Could not set period");
    close(fd);

    fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
    test(fd != 1, "Could not open /sys/class/pwm/pwmchip0/pwm0/enable");
    test(write(fd, "1", 1) == 1, "Could not enable LEDs");
    close(fd);

    setCycle(0.0);
}

void PinControl::stop(){
    setCycle(0);
}


void PinControl::setCycle(float dutyCycle){
    if(dutyCycle > 1 || dutyCycle < 0){
        throw "Duty cycle out of bounds";
        exit(1);
    }
    std::string dc_str = std::to_string((int)((float)(periodms*1e+6)*dutyCycle));

    int fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    test(fd != 1, "Could not open /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
    test(write(fd, dc_str.c_str(), dc_str.size()) == dc_str.size(), "Could not update duty cycle");
    close(fd);
}