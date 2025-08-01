cmake_minimum_required(VERSION 3.31.6)
project(Islamic_Prayer_Timings)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

# Add the executable target
add_executable(Islamic_Prayer_Timings
        main.cpp
        include/utils.h
        src/utils.cpp
        include/prayer_timings.h
        src/prayerTimings.cpp
)

# Find CURL and libnotify
find_package(CURL REQUIRED)

# Find libnotify using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)

# Link the libraries
target_link_libraries(Islamic_Prayer_Timings CURL::libcurl ${LIBNOTIFY_LIBRARIES})

# Include libnotify headers
target_include_directories(Islamic_Prayer_Timings PRIVATE ${LIBNOTIFY_INCLUDE_DIRS})

