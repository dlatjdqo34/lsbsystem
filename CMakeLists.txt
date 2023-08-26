cmake_minimum_required(VERSION 3.22.1)

project(lsbsystem)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "-Wall -g  -lm -lrt -Wl,--no-as-needed -ldl -lseccomp")

include_directories (
    embedded 
    embedded/utils
    embedded/system 
    embedded/ui
    embedded/web
    # drivers
)

add_executable (
    ${PROJECT_NAME} 
    embedded/main.c 
    embedded/utils/utils.c 
    embedded/utils/mq.c 
    embedded/ui/input.c
    embedded/ui/ui.c
    embedded/system/system_server.c 
    embedded/web/web_server.c
    # drivers/test_driver.c
)