cmake_minimum_required(VERSION 3.10)
project(my_todo_list)

set(CMAKE_C_STANDARD 99)

# 设置源文件目录
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 正确查找 ncurses 库
set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)

# 收集源文件
file(GLOB SOURCES 
    "${SRC_DIR}/*.c"
    "main.c"
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIR}
    ${CURSES_INCLUDE_DIR}
)

# 正确链接 ncurses
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

# 编译选项
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# macOS 特殊处理
if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
endif()
