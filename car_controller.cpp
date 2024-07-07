#include "car_controller.h"

#include <cmath>
#include <stdio.h>
#include <string.h>
#include <stdexcept>

#include "gflags.h"

CarController::CarController() noexcept {
  serial_port_ = new SerialPort("/dev/ttyTHS1");
  serial_port_->Open(SerialPort::BAUD_115200);
  // Only for test serial read.
  // LOG(INFO) << serial_port.ReadLine();
  speed_limit_ = FLAGS_SpeedLimit;
  command_.resize(11);
  memset(command_.data(), 0, command_.size());
  command_[0] = 0xff;
  command_[1] = 0xfe;
  command_[2] = ControlMode::CoordinateMode;
}

CarController::~CarController() noexcept {
  serial_port_->Close();
  delete (serial_port_);
}

void CarController::SetMoveParam(int x, int y, int z, int speed_control) {
  ChangeSpeed(speed_control);
  int x_flag = (x >= 0 ? 0 : 1);
  int y_flag = (y >= 0 ? 0 : 1);
  int z_flag = (z >= 0 ? 0 : 1);
  x = abs(x);
  y = abs(y);
  z = abs(z);
  if (x > speed_limit_)
    x = speed_limit_;
  if (y > speed_limit_)
    y = speed_limit_;
  // if (z > speed_limit_) z = speed_limit_;
  command_[3] = x / 256;
  command_[4] = x % 256;
  command_[5] = y / 256;
  command_[6] = y % 256;
  command_[7] = z / 256;
  command_[8] = z % 256;
  command_[9] = x_flag << 2 | y_flag << 1 | z_flag;
}

void CarController::ChangeSpeed(int speed_control) {
  if (speed_control == 1) {
    if (speed_limit_ < 4)
      speed_limit_++;
    else
      speed_limit_ *= 1.4;
  } else if (speed_control == 0) {

  } else if (speed_control == -1) {
    speed_limit_ /= 1.4;
  }
  printf("Speedlimit: %d\n", speed_limit_);
  if (speed_limit_ >= 32768)
    speed_limit_ = 32767;
  if (speed_limit_ < 1)
    speed_limit_ = 1;
}

void CarController::WriteCommand() noexcept {
  try {
    serial_port_->Write(GetCommand());
  } catch(std::exception& e) {
    printf("serial write failed: %s\n", e.what());
    exit(-1);
  }
}