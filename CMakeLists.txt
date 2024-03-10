# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

list(APPEND EXTRA_COMPONENT_DIRS "esp32-fft/components/esp32_fft")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32-fft-benchmarks)
