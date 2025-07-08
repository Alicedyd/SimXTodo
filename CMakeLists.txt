cmake_minimum_required(VERSION 3.10)
project(SimXTodo C)

# 设置C标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置默认构建类型为Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 设置编译器标志
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # GCC 和 Clang 编译器
    set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -DDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
    
    # 添加额外的警告标志
    add_compile_options(-Wall -Wextra -Wpedantic)
    
    # 如果是GCC，添加更多警告
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wformat=2 -Wstrict-prototypes)
    endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # Microsoft Visual C++ 编译器
    set(CMAKE_C_FLAGS_DEBUG "/Od /Zi /RTC1 /MDd /DDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "/O2 /MD /DNDEBUG")
    
    # 禁用一些MSVC特定的警告
    add_compile_options(/wd4996 /wd4244 /wd4267)
    
    # 启用UTF-8源码编码
    add_compile_options(/utf-8)
endif()

# 源文件
set(SOURCES
    main.c
    src/todo_list.c
    src/utils.c
    src/window_control.c
)

# 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 平台特定配置
if(WIN32)
    # Windows 平台配置
    message(STATUS "Configuring for Windows")
    
    # 定义Windows宏
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32)
    
    # 如果是MinGW编译器，添加特定标志
    if(MINGW)
        target_compile_definitions(${PROJECT_NAME} PRIVATE __USE_MINGW_ANSI_STDIO=1)
    endif()
    
    # 查找PDCurses库
    find_path(PDCURSES_INCLUDE_DIR 
        NAMES curses.h panel.h
        PATHS 
            ${CMAKE_CURRENT_SOURCE_DIR}/deps/PDCurses
            ${CMAKE_CURRENT_SOURCE_DIR}/external/PDCurses
            $ENV{PDCURSES_ROOT}/include
            $ENV{PDCURSES_ROOT}
            ${PDCURSES_ROOT}/include
            ${PDCURSES_ROOT}
        PATH_SUFFIXES include
    )
    
    find_library(PDCURSES_LIBRARY
        NAMES pdcurses libpdcurses
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/deps/PDCurses
            ${CMAKE_CURRENT_SOURCE_DIR}/external/PDCurses
            $ENV{PDCURSES_ROOT}/lib
            $ENV{PDCURSES_ROOT}
            ${PDCURSES_ROOT}/lib
            ${PDCURSES_ROOT}
        PATH_SUFFIXES lib wincon
    )
    
    if(PDCURSES_INCLUDE_DIR AND PDCURSES_LIBRARY)
        message(STATUS "Found PDCurses: ${PDCURSES_LIBRARY}")
        target_include_directories(${PROJECT_NAME} PRIVATE ${PDCURSES_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} ${PDCURSES_LIBRARY})
    else()
        message(WARNING "PDCurses not found. Please install PDCurses or set PDCURSES_ROOT.")
        message(STATUS "You can download PDCurses from: https://pdcurses.org/")
        message(STATUS "Or install via vcpkg: vcpkg install pdcurses")
    endif()
    
    # 链接Windows系统库
    target_link_libraries(${PROJECT_NAME} shell32 user32)
    
    # 设置Windows可执行文件属性
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE FALSE
            LINK_FLAGS "/SUBSYSTEM:CONSOLE"
        )
    endif()
    
elseif(APPLE)
    # macOS 平台配置
    message(STATUS "Configuring for macOS")
    
    # 定义macOS宏
    target_compile_definitions(${PROJECT_NAME} PRIVATE __APPLE__)
    
    # 查找ncurses库
    find_package(Curses REQUIRED)
    if(CURSES_FOUND)
        message(STATUS "Found ncurses: ${CURSES_LIBRARIES}")
        target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
    endif()
    
    # 链接macOS框架
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    if(COREFOUNDATION_LIBRARY)
        target_link_libraries(${PROJECT_NAME} ${COREFOUNDATION_LIBRARY})
    endif()
    
    # 设置macOS最低版本
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12")
    
else()
    # Linux 和其他Unix-like系统
    message(STATUS "Configuring for Linux/Unix")
    
    # 查找ncurses库
    find_package(Curses REQUIRED)
    if(CURSES_FOUND)
        message(STATUS "Found ncurses: ${CURSES_LIBRARIES}")
        target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
    endif()
    
    # 链接数学库（某些Linux发行版需要）
    target_link_libraries(${PROJECT_NAME} m)
    
    # 启用GNU扩展
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GNU_SOURCE)
endif()

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
)

# 安装配置
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    COMPONENT Runtime
)

# 创建配置汇总
message(STATUS "")
message(STATUS "=================== Build Configuration Summary ===================")
message(STATUS "Project Name:      ${PROJECT_NAME}")
message(STATUS "Build Type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler:        ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "C Standard:        C${CMAKE_C_STANDARD}")
message(STATUS "System:            ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Architecture:      ${CMAKE_SYSTEM_PROCESSOR}")

if(WIN32)
    message(STATUS "Terminal Library:  PDCurses")
    if(PDCURSES_INCLUDE_DIR)
        message(STATUS "PDCurses Include:  ${PDCURSES_INCLUDE_DIR}")
    endif()
    if(PDCURSES_LIBRARY)
        message(STATUS "PDCurses Library:  ${PDCURSES_LIBRARY}")
    endif()
else()
    message(STATUS "Terminal Library:  ncurses")
    if(CURSES_INCLUDE_DIRS)
        message(STATUS "ncurses Include:   ${CURSES_INCLUDE_DIRS}")
    endif()
    if(CURSES_LIBRARIES)
        message(STATUS "ncurses Library:   ${CURSES_LIBRARIES}")
    endif()
endif()

message(STATUS "C Flags Debug:     ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C Flags Release:   ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "Install Prefix:    ${CMAKE_INSTALL_PREFIX}")
message(STATUS "================================================================")
message(STATUS "")

# 可选：添加自定义目标用于清理和重建
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMENT "Cleaning all build files"
)

# 可选：添加打包支持
set(CPACK_PACKAGE_NAME "SimXTodo")
set(CPACK_PACKAGE_VERSION "0.1.2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A lightweight, cross-platform terminal-based todo list manager")
set(CPACK_PACKAGE_VENDOR "Alicedyd")
set(CPACK_PACKAGE_CONTACT "your-email@example.com")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "SimXTodo")
    set(CPACK_NSIS_PACKAGE_NAME "SimXTodo")
elseif(APPLE)
    set(CPACK_GENERATOR "ZIP;DragNDrop")
else()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libncurses5")
    set(CPACK_RPM_PACKAGE_REQUIRES "ncurses")
endif()

include(CPack)

# 开发者工具：代码格式化（如果有clang-format）
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting source code with clang-format"
    )
endif()

# 开发者工具：静态分析（如果有cppcheck）
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(check
        COMMAND ${CPPCHECK} --enable=all --std=c99 --suppress=missingIncludeSystem ${SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running static analysis with cppcheck"
    )
endif()
