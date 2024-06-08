#include <vector>
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
  CarController();
  void SetMoveParam(int x, int y, int z = 0);
  std::vector<unsigned char> GetCommand() { return command_; }
 private:
  std::vector<unsigned char> command_;
  int speed_limit_ = 0;
};