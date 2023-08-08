cmake_minimum_required(VERSION 3.22.1)

project(lsbsystem)

include_directories (
    embedded 
    embedded/system 
    embedded/ui 
    embedded/utils
)

add_executable (
    ${PROJECT_NAME} 
    embedded/main.c 
    embedded/system/system_server.c 
    embedded/ui/ui.c
    embedded/ui/input.c
    embedded/utils/utils.c 
)