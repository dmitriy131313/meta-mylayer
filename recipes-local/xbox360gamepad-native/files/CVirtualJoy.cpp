#include "CVirtualJoy.h"

#include "suinput.h"
#include <cstring>
#include <iostream>

int CVirtualJoy::static_deviceid = 0;
const int CVirtualJoy::BUTTONS_defs[static_cast<int>(CVirtualJoy::BUTTONS::BUTTONS_counter)] = {
    BTN_A,
    BTN_B,
    BTN_X,
    BTN_Y,
    BTN_TL,
    BTN_TR,
    BTN_TL2,
    BTN_TR2,
    BTN_SELECT,
    BTN_START,
    BTN_THUMBL,
    BTN_THUMBR
};
const int CVirtualJoy::AXES_defs[static_cast<int>(CVirtualJoy::AXES::AXES_counter)] = {
    ABS_X,
    ABS_Y,
    ABS_Z,
    ABS_RX,
    ABS_RY,
    ABS_RZ,
    ABS_HAT0X,
    ABS_HAT0Y
};

CVirtualJoy::CVirtualJoy()
    : deviceid(-1)
    , fd(-1) 
    , dev_started(false)
{
    deviceid = static_deviceid;

    fd = suinput_open();
    if (fd < 0)
    {
        std::cout << "ERROR " << fd << ": Failed opening virtual device " << deviceid << ".\n";
        return;
    }

    static_deviceid++;
}

CVirtualJoy::~CVirtualJoy()
{
    suinput_destroy(fd);
}

bool CVirtualJoy::start(const char* name, unsigned short vendor, unsigned short product, unsigned short version)
{
    struct uinput_user_dev user_dev = {};
    std::string dName = name + std::to_string(deviceid);
    strcpy(user_dev.name, dName.c_str());

    for (unsigned int i = 0; i < ABS_CNT; i++)
    {
        user_dev.absmin[i] = 0;
        user_dev.absmax[i] = 256;
        user_dev.absfuzz[i] = 0;
        user_dev.absflat[i] = 0;
    }

    user_dev.id.bustype = BUS_USB;
    user_dev.id.vendor = vendor;
    user_dev.id.product = product;
    user_dev.id.version = version;

    int r = suinput_create(fd, &user_dev);
    if (r)
    {
        std::cout << "ERROR " << r << ": Failed creating virtual device " << deviceid << ".\n";
        suinput_destroy(fd);
        fd = -1;
        return false;
    } // if

    std::cout << "Successfully created virtual device " << deviceid << ".\n";
    return true;
}

bool CVirtualJoy::isOpen()
{
    return fd >= 0;
}

int CVirtualJoy::getDeviceid()
{
    return deviceid;
}

void CVirtualJoy::enableButtonEvent(BUTTONS but)
{
    if (suinput_enable_event(fd, EV_KEY, BUTTONS_defs[static_cast<int>(but)]) < 0)
    {
        std::cout << "ERROR: Failed enabling event for button " << BUTTONS_defs[static_cast<int>(but)] << " on virtual device << " << deviceid << ".\n";
    }
}

void CVirtualJoy::enableAxisEvent(AXES axis)
{
    if (suinput_enable_event(fd, EV_ABS, AXES_defs[static_cast<int>(axis)]) < 0)
    {
        std::cout << "ERROR: Failed enabling event for axis " << AXES_defs[static_cast<int>(axis)] << " on virtual device << " << deviceid << ".\n";
    }
}

void CVirtualJoy::send_button_event(BUTTONS but, int value)
{
    suinput_emit(fd, EV_KEY, BUTTONS_defs[static_cast<int>(but)], value);
    suinput_syn(fd);
}

void CVirtualJoy::send_axis_event(AXES axis, int value)
{
    suinput_emit(fd, EV_ABS, AXES_defs[static_cast<int>(axis)], value);
    suinput_syn(fd);
}