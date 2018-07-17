cmake_minimum_required(VERSION 3.10)
project(GHPlayer C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/local/Cellar/ffmpeg/3.4.2/include/)
link_directories(/usr/local/Cellar/ffmpeg/3.4.2/lib/)

add_executable(GHPlayer main.c)

target_link_libraries(
        GHPlayer
        avformat
        avcodec
        avdevice
        avfilter
        avresample
        avutil
        postproc
        swresample
        swscale
)