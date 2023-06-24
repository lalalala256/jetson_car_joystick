#include <errno.h>
#include <fcntl.h>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include <glog/logging.h>
#include <SerialPort.h>

#include "car_controller.h"
#include "joystick_xbox.h"

int main()
{
    SerialPort serial_port("/dev/ttyTHS1");
    serial_port.Open(SerialPort::BAUD_115200);
    // Only for test serial read.
    // LOG(INFO) << serial_port.ReadLine();
    CarController car_controller;

	bool ret = false;
    int err_cnt = 0;
    fd_set rfds;
    timeval timeout;
    struct js_event js;
    int fd = -1;
    std::string dev_name = "/dev/input/js0";
    std::unique_ptr<JoystickXBox> joystick_xbox = std::make_unique<JoystickXBox>(dev_name);

    ret = joystick_xbox->Open();
    if (!ret)
    {
        return -1;
    }
    fd = joystick_xbox->GetFd();

    while (1)
    {
        usleep(1000);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        int ret = select(fd + 1, &rfds, NULL, NULL, &timeout);
        if (ret > 0 && FD_ISSET(fd, &rfds))
        {
            ret = joystick_xbox->Read(js);
            if (ret)
            {
                joystick_xbox->ProcessData(js);
                Coor coor = joystick_xbox->GetCommand();
                car_controller.SetMoveParam(coor.y, -coor.x, coor.yaw);
                serial_port.Write(car_controller.GetCommand());
            }
        }
    }

    joystick_xbox->Close();
    serial_port.Close();
    return 0;
}

