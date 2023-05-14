
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
 private:
  unsigned char command[10];
};