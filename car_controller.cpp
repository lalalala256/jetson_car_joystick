#include "car_controller.h"

CarController::CarController() {
  command[0] = 0xff;
  command[1] = 0xfe;
  command[2] = ControlMode::CoordinateMode;
}

void CarController::SetMoveParam(int x, int y, int z) {
  int x_flag = x >= 0 ? 1 : -1;
  int y_flag = y >= 0 ? 1 : -1;
  int z_flag = z >= 0 ? 1 : -1;
  command[3] = x / 256;
  command[4] = x % 256;
  command[5] = y / 256;
  command[6] = y % 256;
  command[7] = z / 256;
  command[8] = z % 256;
  command[9] = x_flag << 2 || y_flag << 1 || z_flag; 
}

