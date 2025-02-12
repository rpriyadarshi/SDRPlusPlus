#include <imgui.h>
#include <module.h>
#include <gui/gui.h>
#include <core.h>
#include <gpio_interface.h>

SDRPP_MOD_INFO{
    /* Name:            */ "gpio",
    /* Description:     */ "GPIO controller",
    /* Author:          */ "Rohit Priyadarshi",
    /* Version:         */ 0, 1, 0,
    /* Max instances    */ 1
};

ConfigManager config;

class GpioModule : public ModuleManager::Instance {
public:
    GpioModule(std::string name) {
        this->name = name;
        gui::menu.registerEntry(name, menuHandler, this, NULL);
        core::modComManager.registerInterface("gpio", name, moduleInterfaceHandler, this);
        GPIO_INFO::getInstance().start(nullptr, nullptr);
   }

    ~GpioModule() {
        gui::menu.removeEntry(name);
        core::modComManager.unregisterInterface(name);
        GPIO_INFO::getInstance().stop();
    }

    void postInit() {
        GPIO_INFO::getInstance().setMode(pin, PI_OUTPUT);
    }

    void enable() {
        enabled = true;
    }

    void disable() {
        enabled = false;
    }

    bool isEnabled() {
        return enabled;
    }

private:
    static void menuHandler(void* ctx) {
        GpioModule* _this = (GpioModule*)ctx;
        ImGui::Text("SDR++ %s", _this->name.c_str());
    }

    static void moduleInterfaceHandler(int code, void* in, void* out, void* ctx) {
        GpioModule* _this = (GpioModule*)ctx;
        std::lock_guard lck(_this->rpgpioMtx);
        switch(code) {
        case GPIO_INFO::KEY_0:
            spdlog::info("GPIO '[{0}->{1}]': KEY_0!", _this->name, (*(std::string*)in));
            GPIO_INFO::getInstance().write(_this->pin, 1);
            break;
        case GPIO_INFO::KEY_1:
            spdlog::info("GPIO '[{0}->{1}]': KEY_1!", _this->name, (*(std::string*)in));
            GPIO_INFO::getInstance().write(_this->pin, 0);
            break;
        }
    }

    std::string name;
    bool enabled = true;
    std::mutex rpgpioMtx;
    unsigned int pin = 18;
};

MOD_EXPORT void _INIT_() {
    // Nothing here
}

MOD_EXPORT ModuleManager::Instance* _CREATE_INSTANCE_(std::string name) {
    GPIO_INFO::getInstance().name = name;
    return new GpioModule(name);
}

MOD_EXPORT void _DELETE_INSTANCE_(void* instance) {
    delete (GpioModule*)instance;
}

MOD_EXPORT void _END_() {
    // Nothing here
}