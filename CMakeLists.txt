cmake_minimum_required(VERSION 3.10)

# sudo apt install libqt5x11extras5-dev

project(AdHole)

# Find Qt5
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 20)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -flto")

# Set sources
set(SOURCES
    main.cpp
    adhole.cpp
)

# Set headers
set(HEADERS
    adhole.hpp
)

# Create executable
add_executable(AdHole ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(AdHole Qt5::Widgets)
target_link_libraries(AdHole systemd)
