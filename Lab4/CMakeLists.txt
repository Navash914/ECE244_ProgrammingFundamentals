
cmake_minimum_required(VERSION 3.12)
project(Lab4_GalaxyExplorer)

set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)

set(CMAKE_CXX_FLAGS -m64)

set(CMAKE_CXX_STANDARD 11)
set(SFML_DIR "C:/MinGW/SFML/lib/cmake/SFML")

find_package(SFML 2.5.0 COMPONENTS network audio graphics window system)

add_executable(Lab4_GalaxyExplorer Asteroid.hpp AsteroidList.hpp
        AsteroidsObserver.hpp GameAI.hpp MyAIData.hpp
        AsteroidList.cpp AsteroidsObserver.cpp GameAI.cpp)

include_directories(C:/Mingw/SFML/include)

link_directories("C:/MinGW/SFML/lib")

target_link_libraries(Lab4_GalaxyExplorer sfml-graphics)

set(CMAKE_CXX_STANDARD 11)

