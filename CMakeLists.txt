project(nano-joystick)

find_package(gflags REQUIRED)

add_executable(nano-joystick joystick_xbox.cpp main.cpp)

target_link_libraries(nano-joystick
  pthread
  gflags
)