#include <errno.h>
#include <fcntl.h>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "joystick_xbox.h"

JoystickXBox::JoystickXBox(const std::string &dev_name) : fd_(-1),dev_name_(dev_name)
{
}

JoystickXBox::~JoystickXBox()
{
    if (axis_)
    {
        delete axis_;
        axis_ = nullptr;
    }

    if (button_)
    {
        delete button_;
        button_ = nullptr;
    }
}

bool JoystickXBox::Open()
{
    int fd = -1;
    if (dev_name_.length() == 0)
    {
        return false;
    }
    // O_NONBLOCK open
    fd = open(dev_name_.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        fd_ = -1;
        printf("JoystickXBox open %s error, %d(%s)\n", dev_name_.c_str(), errno, strerror(errno));
        return false;
    }

    ioctl(fd, JSIOCGVERSION, &version_);
    ioctl(fd, JSIOCGAXES, &axes_);
    ioctl(fd, JSIOCGBUTTONS, &buttons_);
    ioctl(fd, JSIOCGNAME(512), name_);
    printf("JoystickXBox Driver version is %d.%d.%d.\n", version_ >> 16, (version_ >> 8) & 0xff, version_ & 0xff);
    printf("JoystickXBox (%s) has %d axes and %d buttons\n", name_, axes_, buttons_);
    fd_ = fd;
    axis_ = (int *)calloc(axes_, sizeof(int));
    button_ = (char *)calloc(buttons_, sizeof(char));

    return true;
}

void JoystickXBox::Close()
{
    if (fd_ > 0)
    {
        close(fd_);
        fd_ = -1;
    }
}

bool JoystickXBox::Read(struct js_event &js)
{
    int len = -1;
    if (fd_ < 0)
    {
        return false;
    }

    memset(&js, 0, sizeof(js));
    len = read(fd_, &js, sizeof(struct js_event));
    if (len != sizeof(struct js_event))
    {
        printf("JoystickXBox: error reading, %d(%s)\n", errno, strerror(errno));
        return false;
    }

    return true;
}

void JoystickXBox::ProcessData(const struct js_event &js)
{
//    JoystickFrame frame;
    int joystick_angular_value = 0;
    int joystick_linear_value = 0;
    int button_angular_value = 0;
    int button_linear_value = 0;

    switch (js.type & ~JS_EVENT_INIT)
    {
    case JS_EVENT_BUTTON:
        button_[js.number] = js.value;
        break;
    case JS_EVENT_AXIS:
        axis_[js.number] = js.value;
        break;
    }
    if (debug_)
    {
        PrintData();
    }
}

void JoystickXBox::PrintData()
{
    if (axes_ && axis_)
    {
        printf("Axes: ");
        for (int i = 0; i < axes_; i++)
        {
            printf("%2d:%6d ", i, axis_[i]);
        }
    }
    if (buttons_ && button_)
    {
        printf("Buttons: ");
        for (int i = 0; i < buttons_; i++)
        {
            printf("%2d:%s ", i, button_[i] ? "on " : "off");
        }
    }
    printf("\n");
    fflush(stdout);
}
