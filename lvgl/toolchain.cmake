# Specify the cross-compilation toolchain
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)


# Specify the compiler paths
# /home/jiao/Taishan/rope/buildroot/output/rockchip_rk3566/host/bin/aarch64-linux-gnu-g++
# /home/jiao/rv/echo-mate/Echo-Mate/SDK/rv1106-sdk/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf
#set(CMAKE_C_COMPILER /home/jiao/Taishan/rope/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc)
#set(CMAKE_CXX_COMPILER /home/jiao/Taishan/rope/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++)

set(CMAKE_C_COMPILER  gcc)
set(CMAKE_CXX_COMPILER  g++)

# Specify the sysroot (if available)
# set(CMAKE_SYSROOT /home/jiao/Taishan/ubuntu-22/temp)

# Add paths to find libraries and includes
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
