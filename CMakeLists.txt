cmake_minimum_required(VERSION 3.5)

project(LibraryManagement VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 UI 文件目录
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/resources/form)

# 包含目录
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
)

# 查找 Qt 组件
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql REQUIRED)

# 设置源文件
file(GLOB UI_FILES resources/form/*.ui)
file(GLOB HEADER_FILES include/*.h)
file(GLOB SOURCE_FILES src/*.cpp)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LibraryManagement
            MANUAL_FINALIZATION
            ${SOURCE_FILES}
            ${HEADER_FILES}
            ${UI_FILES}
    )
else()
    if(ANDROID)
        add_library(LibraryManagement SHARED
                ${SOURCE_FILES}
                ${HEADER_FILES}
                ${UI_FILES}
        )
    else()
        add_executable(LibraryManagement
                ${SOURCE_FILES}
                ${HEADER_FILES}
                ${UI_FILES}
        )
    endif()
endif()

# 链接库
target_link_libraries(LibraryManagement PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
)

set_target_properties(LibraryManagement PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/resources/form
)


# 复制 dbconfig.ini
configure_file(
        ${CMAKE_SOURCE_DIR}/resources/dbconfig.ini    # 源文件路径
        ${CMAKE_BINARY_DIR}/resources/dbconfig.ini    # 目标路径
        COPYONLY
)

# 复制 remember_me.ini
configure_file(
        ${CMAKE_SOURCE_DIR}/resources/remember_me.ini    # 源文件路径
        ${CMAKE_BINARY_DIR}/resources/remember_me.ini    # 目标路径
        COPYONLY
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LibraryManagement)
endif()