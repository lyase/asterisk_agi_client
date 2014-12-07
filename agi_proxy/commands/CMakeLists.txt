project(commands)
#file(GLOB helloworld_SRC "*.h" "*.cpp" )
#add_library(commandlib STATIC ${helloworld_SRC})
add_library(commands STATIC all.hpp answer.hpp base.hpp channel_status.hpp say_number.hpp streamFile.hpp empty.cpp)

