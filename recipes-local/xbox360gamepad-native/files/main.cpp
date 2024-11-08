#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <map>
#include <signal.h>

#include "CVirtualJoy.h"
#include "UART_drv.h"


#include <stdlib.h>

uint16_t last_button_mask = 0;
bool is_process_alive = true;
UART *uart;
CVirtualJoy *joy;

std::map<uint16_t, CVirtualJoy::BUTTONS> buttons {
    { 0x0001U, CVirtualJoy::BUTTONS::A }, 
    { 0x0002U, CVirtualJoy::BUTTONS::B },
    { 0x0004U, CVirtualJoy::BUTTONS::X },
    { 0x0008U, CVirtualJoy::BUTTONS::Y },
    { 0x0010U, CVirtualJoy::BUTTONS::TL2 },
    { 0x0020U, CVirtualJoy::BUTTONS::TR2 },
    //0x0040U hat up
    //0x0080U hat down
    //0x0100U hat left
    //0x0200U hat right
    { 0x0400U, CVirtualJoy::BUTTONS::TL },
    { 0x0800U, CVirtualJoy::BUTTONS::THUMBL },
    { 0x1000U, CVirtualJoy::BUTTONS::TR },
    { 0x2000U, CVirtualJoy::BUTTONS::THUMBR },
    { 0x4000U, CVirtualJoy::BUTTONS::START },
    { 0x8000U, CVirtualJoy::BUTTONS::SELECT },
};

bool readUartRAW(char *data)
{
    char buf[2] = {0, 0};
    int ret = uart->read(buf, 2);

    if (ret < 0) 
    {
        printf("UART read error\n");
        return false;
    }    

    if (buf[0] == 0xAB && buf[1] == 0xAB)
    {
        int ret = uart->read(data, 6);
        if (ret < 0) printf("UART read error\n");
        return (ret == 6);
    }
    else
    {
        return false;
    }
}

void process_hats(uint16_t *button_mask)
{
    int hat_up    = (0x0040U & *button_mask) ? -1 : 0;
    int hat_down  = (0x0080U & *button_mask) ? 1 : 0;
    int hat_right = (0x0200U & *button_mask) ? 1 : 0;
    int hat_left  = (0x0100U & *button_mask) ? -1 : 0;

    joy->send_axis_event(CVirtualJoy::AXES::HAT0X, 128 * (hat_left + hat_right));
    joy->send_axis_event(CVirtualJoy::AXES::HAT0Y, 128 * (hat_up + hat_down));
}

void process_buttons(char *mask)
{
    uint16_t button_mask = ((uint16_t)mask[1] << 8) | (uint16_t)mask[0];
    for (auto& it : buttons)
    {
        bool curr_lvl = (bool)(it.first & button_mask);
        bool last_lvl = (bool)(it.first & last_button_mask);
        if (curr_lvl != last_lvl)
        {
            joy->send_button_event(it.second, curr_lvl);
        }
    }
    process_hats(&button_mask);
    last_button_mask = button_mask;
}

void process_axises(char *mask)
{
    uint8_t LX = mask[2];
    uint8_t LY = mask[3];
    uint8_t RX  = mask[4];
    uint8_t RY = mask[5];

    if (LX < 133 && LX > 123) LX = 128;
    if (LY < 133 && LY > 123) LY = 128;

    // static int ttt = 0;
    // printf("cnt: %d\n", ttt++);
    // printf("LX: %d\n", LX);
    // printf("LY: %d\n", LY);
    // printf("RX: %d\n", RX);
    // printf("RY: %d\n\n", RY);

    joy->send_axis_event(CVirtualJoy::AXES::X, LX);
    joy->send_axis_event(CVirtualJoy::AXES::Y, LY);
    joy->send_axis_event(CVirtualJoy::AXES::RX,RX);
    joy->send_axis_event(CVirtualJoy::AXES::RY, RY);
    // uint8_t HAT_X = s[5];
    // uint8_t HAT_Y = s[5];
}

int main(int argc, char *argv[])
{
    printf("Enter main\n");
    signal(SIGTERM, [](int sig){if (sig == SIGTERM)
    {
        printf("SIGTERM Received!\n");
        is_process_alive = false;
    }});

    uart = new UART("/dev/ttyS1");
    uart->setAttribs(UART::SPEED::_115200, UART::FLOW_CTRL::NO, UART::DATA_BITS::_8, UART::STOP_BITS::_1, UART::PARITY::NO);

    joy = new CVirtualJoy();

    for (auto& it : buttons)
        joy->enableButtonEvent(it.second);    

    joy->enableAxisEvent(CVirtualJoy::AXES::X);
    joy->enableAxisEvent(CVirtualJoy::AXES::Y);
    joy->enableAxisEvent(CVirtualJoy::AXES::Z);
    joy->enableAxisEvent(CVirtualJoy::AXES::RX);
    joy->enableAxisEvent(CVirtualJoy::AXES::RY);
    joy->enableAxisEvent(CVirtualJoy::AXES::RZ);
    joy->enableAxisEvent(CVirtualJoy::AXES::HAT0X);
    joy->enableAxisEvent(CVirtualJoy::AXES::HAT0Y);

    joy->start("Microsoft X-Box 360 pad ", 0x045e, 0x028e, 0x110);

    joy->send_axis_event(CVirtualJoy::AXES::X, 128);
    joy->send_axis_event(CVirtualJoy::AXES::Y, 128);
    joy->send_axis_event(CVirtualJoy::AXES::Z, 128);
    joy->send_axis_event(CVirtualJoy::AXES::RX, 128);
    joy->send_axis_event(CVirtualJoy::AXES::RY, 128);
    joy->send_axis_event(CVirtualJoy::AXES::RZ, 128);
    joy->send_axis_event(CVirtualJoy::AXES::HAT0X, 128);
    joy->send_axis_event(CVirtualJoy::AXES::HAT0Y, 128);

    char raw_data[6];

    while(is_process_alive)
    {
        if (readUartRAW(raw_data))
        {
            last_button_mask = ((uint16_t)raw_data[1] << 8) | (uint16_t)raw_data[0];
            break;
        }
    }

    while(is_process_alive)
    {
        if (!readUartRAW(raw_data)) continue;

        process_buttons(raw_data);
        process_axises(raw_data);
    }

    delete uart;
    delete joy;
    printf("UART and UINPUT device is terminated\n");
	return 0;
}