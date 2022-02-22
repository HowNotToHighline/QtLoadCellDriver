cmake_minimum_required(VERSION 3.5)

project(LoadCellDriver LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)

if(ANDROID)
  add_library(LoadCellDriver SHARED
          src/main.cpp
          src/mainwindow.cpp
          src/mainwindow.h
          src/mainwindow.ui
  )
else()
  add_executable(LoadCellDriver
          src/main.cpp
          src/mainwindow.cpp
          src/mainwindow.h
          src/mainwindow.ui
          src/DataAcquisition/devices/LoadCellDriver.h src/DataAcquisition/DataProcessor.cpp src/DataAcquisition/DataProcessor.h src/DataAcquisition/devices/VirtualLoadCell.cpp src/DataAcquisition/devices/VirtualLoadCell.h src/DataAcquisition/devices/LabJackDriver.cpp src/DataAcquisition/devices/LabJackDriver.h src/CallbackInterface.h)
endif()

target_link_libraries(LoadCellDriver PRIVATE Qt5::Widgets Threads::Threads LabJackM)
