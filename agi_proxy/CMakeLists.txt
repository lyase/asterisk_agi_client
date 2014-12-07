add_subdirectory(commands)
project(agi_proxy)
file(GLOB helloworld_SRC
    "*.h"
    "*.cpp"
"agi_proxy/commands/*.* "
)
#add_library(agi_proxy proxy.cpp ${helloworld_SRC})
add_library(agi_proxy empty.cpp)
target_link_libraries(agi_proxy commandslib )

