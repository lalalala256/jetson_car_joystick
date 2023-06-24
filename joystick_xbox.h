#include <linux/input.h>
#include <linux/joystick.h>
#include <string>

struct Coor  {
  int x = 0;
  int y = 0;
};
class JoystickXBox
{
public:
    JoystickXBox(const std::string &dev_name);
    ~JoystickXBox();
    bool Open();
    void Close();
    bool Read(struct js_event &js);
    unsigned char GetAxes()
    {
        return axes_;
    }
    unsigned char GetButtons()
    {
        return buttons_;
    }
    int GetFd()
    {
   		return fd_;
    }
    void PrintData();
    void ProcessData(const struct js_event &js);

    Coor GetCommand();
    void SpeedLimitControl();
private:
    bool debug_ = true;
    int fd_ = -1;
    std::string dev_name_ = "";
    int version_ = 0x000800;
    char name_[512] = "Unkown";
    unsigned char axes_ = 2;
    unsigned char buttons_ = 2;
    int *axis_ = nullptr;
    char *button_ = nullptr;
};

