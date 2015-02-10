cmake_minimum_required(VERSION 2.8.3)

project(jimmy)

find_package(orocos_kdl REQUIRED)
find_package(catkin REQUIRED COMPONENTS geometry_msgs
					roscpp
					roslib
					tf
					rospy
					std_msgs
					sensor_msgs
					cv_bridge
					image_transport
					message_generation
					)

# Find OpenNI
find_package(PkgConfig)
pkg_check_modules(OpenNI REQUIRED libopenni)

#Find GLUT
find_package(GLUT REQUIRED)

add_message_files(
	FILES
	jimmy_command.msg
	jimmy_gesture.msg
	jimmy_servo.msg
	pointerpos.msg
	gestures.msg
)

generate_messages(
	DEPENDENCIES	
	geometry_msgs
	std_msgs
	sensor_msgs
)

catkin_package(
	CATKIN_DEPENDS message_runtime std_msgs sensor_msgs)

include_directories(${catkin_INCLUDEDIR}
		    ${OpenNI_INCLUDEDIR}
		    ${orocos_kdl_INCLUDE_DIRS}
		    ${OpenCV_INCLUDE_DIRS}
		    ${OpenGL_INCLUDE_DIRS}
		    ${GLUT_LIBRARIES})


include_directories("nite/Include")
include_directories("nite/Source/XnCommon")
include_directories("${PROJECT_SOURCE_DIR}/include/jimmy")
link_directories("${PROJECT_SOURCE_DIR}/lib")

add_library(jimmylib SHARED "src/IK.cpp" "src/lipm_planner.cpp" "src/Plan.cpp" "src/SDModel_common.cpp" "src/Jimmy.cpp" "src/Logger.cpp" "src/RobotState.cpp" "src/TrajEW.cpp" "src/mrdplot.c" "src/sdlib.c" "src/ControlUtils.cpp")
  set_target_properties(jimmylib PROPERTIES COMPILE_FLAGS "-O4")



link_directories(${catkin_LIBRARY_DIRS})
link_directories(${orocos_kdl_LIBRARY_DIRS}
		 ${GLUT_LIBRARIES}
)

add_executable(test_jimmy src/JimmyWalkMain.cpp)
add_dependencies(test_jimmy geometry_msgs_gencpp ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(test_jimmy 
				   ${catkin_LIBRARIES}
				   ${OpenNI_LIBRARIES}
				   ${orocos_kdl_LIBRARIES}
				   ${OpenGL_INCLUDE_DIRS} 
				jimmylib rt dxl -lXnVNite_1_5_2) #dxl

add_executable(detectfinger src/SingleControl/main.cpp)
add_dependencies(detectfinger geometry_msgs_gencpp ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(detectfinger ${catkin_LIBRARIES}
				   ${OpenNI_LIBRARIES}
				   ${orocos_kdl_LIBRARIES}
				   ${OpenGL_INCLUDE_DIRS} -lXnVNite_1_5_2)


install(TARGETS test_jimmy detectfinger
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

install(DIRECTORY include/${PROJECT_NAME}/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(DIRECTORY launch/
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
	PATTERN ".svn" EXCLUDE)

install(FILES jimmy.xml 
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
