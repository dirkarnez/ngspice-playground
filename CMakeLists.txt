cmake_minimum_required(VERSION 3.18)

project("ngspice-playground")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

find_package(ngspice REQUIRED) # PATHS "P:\\Downloads\\cpp-libraries\\portaudio\\build\\cmake\\portaudio")

IF(ngspice_FOUND)
    message(STATUS "YES")
ELSE(ngspice_FOUND)
    message(STATUS "NO")
ENDIF(ngspice_FOUND)

add_executable(${PROJECT_NAME}
    "src/main.cpp"
)

target_link_libraries(${PROJECT_NAME} ngspice)