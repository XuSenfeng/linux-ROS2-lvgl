#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include <src/core/lv_obj_event.h>
#include <src/core/lv_obj_pos.h>
#include <src/lv_api_map_v9_1.h>
#include <src/misc/lv_event.h>
#include <src/misc/lv_types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include "gui_app/common/lv_lib.h"
#include "gui_app/ui.h"

#include "rclcpp/rclcpp.hpp"

static const char *getenv_default(const char *name, const char *dflt)
{
    return getenv(name) ? : dflt;
}

#if LV_USE_EVDEV
// 实际开发板运行的
// https://docs.lvgl.io/9.2/integration/driver/touchpad/evdev.html
static void lv_linux_indev_init(void)
{
    lv_indev_t * touch;
    // 设置使用的时间设备
    touch = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event1");
}
#endif

#if LV_USE_LINUX_FBDEV
// 开发板使用
static void lv_linux_disp_init(void)
{
    // 初始化一个屏幕设备
    const char *device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
    lv_display_t * disp = lv_linux_fbdev_create();

    lv_linux_fbdev_set_file(disp, device);
}
#elif LV_USE_LINUX_DRM
static void lv_linux_disp_init(void)
{
    const char *device = getenv_default("LV_LINUX_DRM_CARD", "/dev/dri/card0");
    lv_display_t * disp = lv_linux_drm_create();

    lv_linux_drm_set_file(disp, device, -1);
}
#elif LV_USE_SDL
// Linux下面模拟使用
static void lv_linux_disp_init(void)
{
    const int width = atoi(getenv("LV_SDL_VIDEO_WIDTH") ? : "480");
    const int height = atoi(getenv("LV_SDL_VIDEO_HEIGHT") ? : "800");

    lv_sdl_window_create(width, height);
}

static void lv_linux_indev_init(void)
{
    lv_sdl_mouse_create();
}

#else
#error Unsupported configuration
#endif


void button_cb1(lv_event_t * e)
{
    static bool state = false;
    state = !state;
    lv_obj_t * label = (lv_obj_t *)lv_event_get_user_data(e);
    if(state)
        lv_label_set_text(label, "Normal");
    else
        lv_label_set_text(label, "Clicked");
}

class HelloWorldNode : public rclcpp::Node
{	
    // ROS2节点初始化以及父类初始化
    public:
        HelloWorldNode()
        : Node("node_helloworld_class")
        {
            using namespace std::chrono_literals;
            printf("HelloWorldNode started.\n");
            timer_ = this->create_wall_timer(
                1s,
                [this]() {
                    printf("helloworld\n");
                    RCLCPP_INFO(this->get_logger(), "Hello World");
                }
            );
        }

    private:
        rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char **argv)
{
    lv_init();

    /*Linux display device init*/
    lv_linux_disp_init();

    lv_linux_indev_init();

    /*Create a Demo*/
    // lv_demo_widgets();
    // lv_demo_widgets_start_slideshow();
    printf("Initializing UI...\n");
    ui_init();

    rclcpp::init(argc, argv);
    auto node = std::make_shared<HelloWorldNode>();
    rclcpp::executors::SingleThreadedExecutor executor;
    // executor.add_node(node);

    /*Handle LVGL tasks*/
    while(rclcpp::ok()) {
        executor.spin_some();
        uint32_t sleep_ms = lv_timer_handler();
        if(sleep_ms < 1) {
            sleep_ms = 1;
        }
        usleep(sleep_ms * 1000);
    }
    rclcpp::shutdown();

    return 0;
}
