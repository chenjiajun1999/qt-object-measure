cmake_minimum_required(VERSION 3.5)
project(${PROJECT_NAME})


set(CMAKE_INCLUDE_CURRENT_DIR ON)

# init qt
set(CMAKE_AUTOMOC ON) # meta object compiler
set(CMAKE_AUTORCC ON) # resource files
# set(CMAKE_AUTOUIC ON) # UI files

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D FREEGLUT_STATIC")

# find Qt package
#set(QT_VERSION 6)
#set(CMAKE_PREFIX_PATH "C:/Environment/qt/6.2.2/mingw_64/lib/cmake")
set(QT_VERSION 5)
set(CMAKE_PREFIX_PATH "C:/Environment/qt/5.12.12/mingw73_64/lib/cmake")
find_package(Qt${QT_VERSION} COMPONENTS Gui Core Widgets Quick QuickControls2 Multimedia Xml Concurrent REQUIRED)

# find OpenCV package
set(OpenCV_DIR "C:/Environment/opencv-4.5.2-mingw-build")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

# find source files
file(GLOB_RECURSE DIR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE DIR_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE DIR_HEADLESS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE DIR_QRC ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)

add_executable(
        ${PROJECT_NAME}
        ${DIR_SRCS}
        ${DIR_HEADERS}
        ${DIR_HEADLESS}
        ${DIR_QRC})

target_link_libraries(
        ${PROJECT_NAME}
        ${OpenCV_LIBS}
        Qt${QT_VERSION}::Gui
        Qt${QT_VERSION}::Core
        Qt${QT_VERSION}::Widgets
        Qt${QT_VERSION}::Quick
        Qt${QT_VERSION}::Multimedia
        Qt${QT_VERSION}::QuickControls2
        Qt${QT_VERSION}::Xml
        Qt${QT_VERSION}::Concurrent
        opengl32)


