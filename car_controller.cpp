#include "car_controller.h"

#include <string.h>
#include <cmath>
#include <stdio.h>

#include "gflags.h"

CarController::CarController() {
  speed_limit_ = FLAGS_SpeedLimit;
  command_.resize(11);
  memset(command_.data(), 0, command_.size());
  command_[0] = 0xff;
  command_[1] = 0xfe;
  command_[2] = ControlMode::CoordinateMode;
}

void CarController::SetMoveParam(int x, int y, int z) {
  int x_flag = (x >= 0 ? 0 : 1);
  int y_flag = (y >= 0 ? 0 : 1);
  int z_flag = (z >= 0 ? 0 : 1);
  x = abs(x);
  y = abs(y);
  z = abs(z);
  if (x > speed_limit_) x = speed_limit_;
  if (y > speed_limit_) y = speed_limit_;
  // if (z > speed_limit_) z = speed_limit_;
  command_[3] = x / 256;
  command_[4] = x % 256;
  command_[5] = y / 256;
  command_[6] = y % 256;
  command_[7] = z / 256;
  command_[8] = z % 256;
  command_[9] = x_flag << 2 | y_flag << 1 | z_flag;
}

