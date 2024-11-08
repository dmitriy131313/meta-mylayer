#pragma once
#include <string>

class UART
{
public:
    enum class SPEED
    {
        _9600 = 0000015,   //B9600 in termius.h
        _115200 = 0010002, //B115200 in termius.h
    };

    enum class FLOW_CTRL
    {
        NO,
        HW,
        SW
    };

    enum class DATA_BITS
    {
        _5 = 0000000, //CS5 in termios-c_cflag.h
        _6 = 0000020, //CS6 in termios-c_cflag.h
        _7 = 0000040, //CS7 in termios-c_cflag.h
        _8 = 0000060  //CS8 in termios-c_cflag.h
    };

    enum class STOP_BITS
    {
        _1,
        _2
    };

    enum class PARITY
    {
        NO,
        EVEN,
        ODD,
        SPACE
    };

public:
    UART(const std::string& filename);
    ~UART();

    int setAttribs(SPEED speed, FLOW_CTRL flow_ctrl, DATA_BITS databits, STOP_BITS stopbits, PARITY parity);

    int read(char* data, unsigned int len);
    int write(char* data, unsigned int len);

private:
	int _fd;
};