project(nano-joystick)

find_package(gflags REQUIRED)

add_executable(nano-joystick gflags.cpp joystick_xbox.cpp car_controller.cpp main.cpp)

# pkg-config最简单用法 https://www.jianshu.com/p/eb6b06463da9
find_package(PkgConfig REQUIRED)
pkg_search_module(SERIAL REQUIRED libserial)

target_link_libraries(nano-joystick
  pthread
  gflags
  glog
  ${SERIAL_LIBRARIES}
)