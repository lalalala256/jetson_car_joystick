#include <vector>

#include <SerialPort.h>

/*
    car direction:
       y
       ▲
       │
       │
       └───────► x
*/

enum ControlMode {CoordinateMode = 0x01, MotorSpeedMode = 0x02};

class CarController{
 public:
  CarController() noexcept;
  ~CarController() noexcept;
  void SetMoveParam(int x, int y, int z = 0, int speed_control = 0);
  std::vector<unsigned char> GetCommand() { return command_; }
  // speed_control = 1: speed up
  // speed_control = 0: no change
  // speed_control = -1: speed down
  void WriteCommand() noexcept;
 private:
  std::vector<unsigned char> command_;
  int speed_limit_ = 0;
  SerialPort* serial_port_;

  void ChangeSpeed(int speed_control);
};