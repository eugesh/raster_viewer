cmake_minimum_required(VERSION 3.5)

project(raster_view LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
    #SET(OpenCV_DIR /home/evgeny/soft/opencv/opencv/release)
    LINK_DIRECTORIES(/usr/local/lib/)
endif (UNIX)

find_package(PkgConfig)
find_package(OpenCV REQUIRED)

if(NOT OpenCV_FOUND)
    MESSAGE(FATAL_ERROR "OpenCV not found")
endif()

set(OpenCV_LIBS opencv_core opencv_highgui opencv_imgproc opencv_videoio opencv_calib3d opencv_features2d opencv_xfeatures2d opencv_flann opencv_photo opencv_aruco) #opencv_contrib

find_package(Qt5 COMPONENTS Gui Core Widgets OpenGL PrintSupport REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_add_resources (QRCS raster_view.qrc)

add_executable(raster_view
    main.cpp
    mainwindow.cpp
    image-graphics-scene.cpp
    image-item.cpp
    imageprocessor.cpp
    image-view.cpp ${QRCS})

# Resources:
set(raster_view_resource_files
    "images/fileprint.png "
    "images/qt4logo.png"
    "images/rotateleft.png"
    "images/rotateright.png"
    "images/zoomin.png"
    "images/zoomout.png"
)

#qt5_add_resources(raster_view "raster_view"
#    PREFIX
#        "/"
#    FILES
#        ${raster_view_resource_files}
#)

if (UNIX)
    target_link_libraries(raster_view ${OpenCV_LIBS} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt::PrintSupport stdc++ m)
endif()
