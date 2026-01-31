cmake_minimum_required(VERSION 3.8)
project(lvgl)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# If building for ARM, set the toolchain file before anything else
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake" CACHE FILEPATH "Default toolchain file for ARM builds")
    message(STATUS "No toolchain file specified; using default: ${CMAKE_TOOLCHAIN_FILE}")
endif()
message(STATUS "Building for ARM using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
# 设置交叉编译环境下 pkg-config 使用的路径
set(ENV{PKG_CONFIG_PATH} "${CMAKE_SYSROOT}/usr/lib/pkgconfig:${CMAKE_SYSROOT}/usr/lib/aarch64-linux-gnu/pkgconfig")

set(CMAKE_C_STANDARD 99)#C99 # lvgl officially support C99 and above
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置输出文件夹
set(OUTPUT_PATH ${PROJECT_SOURCE_DIR}/arm-bin)

add_subdirectory(lvgl)          # LVGL 核心模块
add_subdirectory(gui_app)       # GUI_APP 应用模块
add_subdirectory(utils)         # 公共工具模块
add_subdirectory(common)        # common

# Include 配置头文件路径
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR}/conf)

# 定义主程序源文件
set(MAIN_SOURCES main.cpp)

# 链接库
add_executable(main ${MAIN_SOURCES})

# 链接自定义的文件夹库
target_link_libraries(
    main 
    gui_app
    common
)

ament_target_dependencies(main rclcpp)

include(${CMAKE_CURRENT_LIST_DIR}/lvgl/tests/FindLibDRM.cmake)
include_directories(${Libdrm_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)
# 查找libcurl及其依赖
pkg_check_modules(LIBCURL REQUIRED libcurl)
pkg_check_modules(LIBDRM REQUIRED libdrm)

# 添加curl头文件路径
target_include_directories(main PRIVATE ${LIBCURL_INCLUDE_DIRS} ${LIBDRM_INCLUDE_DIRS})
target_include_directories(lvgl PRIVATE ${LIBDRM_INCLUDE_DIRS})

# 添加编译选项
target_compile_options(main PRIVATE ${LIBCURL_CFLAGS_OTHER} ${LIBDRM_CFLAGS_OTHER})
target_compile_options(lvgl PRIVATE ${LIBDRM_CFLAGS_OTHER})

# 关键：链接curl及所有依赖库，解决符号未定义问题
target_link_libraries(main 
    lvgl lvgl::examples lvgl::demos lvgl::thorvg 
    ${Libdrm_LIBRARIES} 
    ${LIBCURL_LIBRARIES}  # curl核心库
    # curl依赖的底层库（解决符号未定义）
    nghttp2 idn2 rtmp ssh psl ssl crypto gssapi_krb5 
    ldap lber zstd brotlidec z m asound pthread dl
)

# 关键：设置rpath，让链接器和运行时找到ARM库
set_target_properties(main PROPERTIES
    LINK_FLAGS "-Wl,-rpath=${ARM_LIB_PATH} -Wl,--enable-new-dtags"
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${ARM_LIB_PATH}"
)


add_custom_target (run COMMAND ${OUTPUT_PATH}/main DEPENDS main)

# 以下内容用于复制配置文件, 在main目标构建后执行, CMake自带的文件复制工具

# 秘钥的配置文件
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/utils/system_para.conf
            ${OUTPUT_PATH}/system_para.conf
    COMMENT "Copying system_para.conf to bin directory"
)

# 高德行政区划代码文件
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/utils/gaode_adcode.json
            ${OUTPUT_PATH}/gaode_adcode.json
    COMMENT "Copying gaode_adcode.json to bin directory"
)

# 根证书文件
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/utils/cacert.pem
            ${OUTPUT_PATH}/cacert.pem
    COMMENT "Copying cacert.pem to bin directory"
)


# 获取所有需要复制的共享库文件
# set(AI_CAMERA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../yolov5_demo" CACHE PATH "Path to AIChat_demo Client")
set(SHARED_LIBS
    # "${AI_CAMERA_PATH}/cpp/3rdparty/librga/Linux/armhf_uclibc/librga.so"
    # "${AI_CAMERA_PATH}/cpp/3rdparty/rknpu2/Linux/armhf-uclibc/librknnmrt.so"
)

# 创建目标目录
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_PATH}/lib
    COMMENT "Creating lib directory for shared libraries"
)

# 复制每个共享库文件
foreach(LIB_FILE ${SHARED_LIBS})
    get_filename_component(FILE_NAME ${LIB_FILE} NAME) # 获取文件名
    add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LIB_FILE}
                ${OUTPUT_PATH}/lib/${FILE_NAME}
        COMMENT "Copying ${FILE_NAME} to executable output directory"
    )
endforeach()
# 设置运行时 rpath，以便可执行文件能够在运行时找到指定目录下的共享库
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,\$ORIGIN/lib:${ARM_LIB_PATH}")


# 使用 set_target_properties 设置 main 的输出目录
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
)

# Custom command to clean up generated files
if(NOT TARGET clean-all)
    add_custom_target(clean-all
        COMMAND find "${CMAKE_BINARY_DIR}" -mindepth 1 -maxdepth 1 -exec rm -rf {} +
        COMMENT "Cleaning all generated files."
    )
endif()

install(TARGETS main 
  DESTINATION lib/${PROJECT_NAME}
  # 可选：添加可执行权限，避免运行时权限不足
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
              GROUP_READ GROUP_EXECUTE
              WORLD_READ WORLD_EXECUTE
)

# 修复配置文件的安装路径（建议同步装到share/包名/，方便ros2 run时读取）
install(FILES arm-bin/cacert.pem arm-bin/gaode_adcode.json arm-bin/system_para.conf 
        DESTINATION lib/${PROJECT_NAME}
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

ament_package()
