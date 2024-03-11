cmake_minimum_required(VERSION 3.5)

project(task1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Find OpenCV
find_package(OpenCV REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        QtWidgetsApplication2.cpp
        QtWidgetsApplication2.h
        QtWidgetsApplication2.ui
        source_code.cpp  # Include source_code.cpp
)

set(HEADERS
    source_code.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(task1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        edgeDetection.h
        edgeDetection.cpp
        source_code.h
    )
else()
    if(ANDROID)
        add_library(task1 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(task1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Add the include directories to the target
target_include_directories(task1 PRIVATE
    ${OpenCV_INCLUDE_DIRS}  # Include OpenCV headers
    ${HEADERS}  # Include your custom headers
)

# Link Qt and OpenCV libraries
target_link_libraries(task1 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OpenCV_LIBS} # Link OpenCV libraries
)

# Set properties
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.task1)
endif()
set_target_properties(task1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS task1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(task1)
endif()
