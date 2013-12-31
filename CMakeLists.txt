cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

include_directories("${PROJECT_SOURCE_DIR}/include/jimmy")

add_library(jimmylib SHARED "src/IK.cpp" "src/lipm_planner.cpp" "src/Plan.cpp" "src/SDModel_common.cpp" "src/Jimmy.cpp" "src/Logger.cpp" "src/RobotState.cpp" "src/TrajEW.cpp" "src/mrdplot.c" "src/sdlib.c" "src/ControlUtils.cpp" "src/soautils.c" "src/serial.cpp")
  set_target_properties(jimmylib PROPERTIES COMPILE_FLAGS "-O4")

rosbuild_add_executable(test_jimmy src/JimmyWalkMain.cpp)
  target_link_libraries(test_jimmy jimmylib rt dxl)

rosbuild_add_executable(test_serial src/test_as_client.cpp)
  target_link_libraries(test_serial jimmylib rt dxl)

rosbuild_add_executable(test_puppet src/test_puppet.cpp)
  target_link_libraries(test_puppet jimmylib rt dxl)

rosbuild_add_executable(test_ros src/test_ros.cpp)
  target_link_libraries(test_ros jimmylib rt dxl)

#rosbuild_add_executable(test_lipm src/lipm_test.cpp)
#  target_link_libraries(test_lipm jimmylib rt dxl)

#rosbuild_add_executable(test_serial src/test_serial.cpp)
#  target_link_libraries(test_serial jimmylib rt dxl)

#rosbuild_add_executable(test_wiggle src/test_wiggle.cpp)
#  target_link_libraries(test_wiggle jimmylib rt dxl)
