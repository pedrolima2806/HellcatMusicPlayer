cmake_minimum_required(VERSION 3.20)
project(hellcat_mp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)

add_executable(hellcat_mp
        src/main.cpp
        src/ui/Button.cpp
        include/ui/Button.h
        include/ui/Widget.h
        include/ui/Widgets.h
        src/audioEngine/musicPlayer.cpp
        include/audioEngine/musicPlayer.h
        src/audioEngine/playlist.cpp
        include/audioEngine/playlist.h
        src/ui/TextRenderer.cpp
        include/ui/TextRenderer.h)

target_include_directories(hellcat_mp PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL_image/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL_ttf/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL_mixer/include"
)

target_link_libraries(hellcat_mp PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3_mixer::SDL3_mixer)
