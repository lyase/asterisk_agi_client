CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT(AGI)

ADD_DEFINITIONS(-std=c++11 -Wall -Wextra)

ENABLE_TESTING()

add_subdirectory(agi_proxy)

ADD_EXECUTABLE(testagi main.cpp)
target_link_libraries(testagi agi_proxy)
SET (test_parameters  <../test_input.txt )
FILE(WRITE ${CMAKE_BINARY_DIR}/main.sh "echo \"PID=\$\$ (shell)\" exec $1 < $2\n")
file(COPY     ${CMAKE_CURRENT_SOURCE_DIR}/test_input.txt     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
ADD_TEST(NAME pipetest
    COMMAND sh ${CMAKE_BINARY_DIR}/main.sh $<TARGET_FILE:testagi> ${CMAKE_BINARY_DIR}/test_input.txt)

install(TARGETS testagi DESTINATION /usr/share/asterisk/agi-bin/testagi PERMISSIONS WORLD_READ WORLD_EXECUTE)
