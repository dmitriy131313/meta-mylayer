#pragma once

class CVirtualJoy
{
public:
    enum class BUTTONS
    {
        A,
        B,
        X,
        Y,
        TL,
        TR,
        TL2,
        TR2,
        SELECT,
        START,
        THUMBL,
        THUMBR,

        BUTTONS_counter
    };

    enum class AXES
    {
        X,
        Y,
        Z,
        RX,
        RY,
        RZ,
        HAT0X,
        HAT0Y,

        AXES_counter
    };

public:
    CVirtualJoy();
    ~CVirtualJoy();

    bool start(const char* name, unsigned short vendor, unsigned short product, unsigned short version);

    void enableButtonEvent(BUTTONS but);
    void enableAxisEvent(AXES axis);

    void send_button_event(BUTTONS but, int value);
    void send_axis_event(AXES axis, int value);
    int getDeviceid();
    bool isOpen();

private:
    int deviceid;
    int fd;
    bool dev_started;
    static int static_deviceid;

private:
    static const int BUTTONS_defs[static_cast<int>(BUTTONS::BUTTONS_counter)];
    static const int AXES_defs[static_cast<int>(AXES::AXES_counter)];

};