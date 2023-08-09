cmake_minimum_required(VERSION 3.22.1)

project(lsbsystem)

include_directories (
    embedded 
    embedded/utils
    embedded/system 
    embedded/ui
    embedded/web 
)

add_executable (
    ${PROJECT_NAME} 
    embedded/main.c 
    embedded/utils/utils.c 
    embedded/ui/input.c
    embedded/ui/ui.c
    embedded/system/system_server.c 
    embedded/web/web_server.c
)