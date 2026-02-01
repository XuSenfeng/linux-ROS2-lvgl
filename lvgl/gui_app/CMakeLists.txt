
file(GLOB_RECURSE GUI_APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.C"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)

# 定义 gui_app 模块为一个静态库
add_library(gui_app STATIC ${GUI_APP_SOURCES})

# 设置 gui_app 的头文件路径
target_include_directories(gui_app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/common/stack
    ${CMAKE_CURRENT_SOURCE_DIR}/common/page_manager
)

# 查找包
find_package(json-c REQUIRED)
find_package(CURL REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

# 将库链接到 gui_app
target_link_libraries(gui_app json-c)
target_include_directories(gui_app PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(gui_app CURL::libcurl)
ament_target_dependencies(gui_app rclcpp geometry_msgs)