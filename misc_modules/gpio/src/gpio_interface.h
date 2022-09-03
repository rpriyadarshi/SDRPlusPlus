#pragma once

#include <pigpio.h>
#include <module.h>
// https://pinout.xyz/

class GPIO_INFO {
public:
    std::string name {"undefined"};
    bool _initialized = false;

private:
    GPIO_INFO() {}
    ~GPIO_INFO() {}

public:
    GPIO_INFO(const GPIO_INFO&) = delete;
    void operator=(const GPIO_INFO&) = delete;

    static GPIO_INFO& getInstance() {
        static GPIO_INFO instance;
        return instance;
    }

    void load(std::string modname) {
        ModuleManager::Module_t mod = core::moduleManager.modules[modname];
        for (auto const& [instname, instance] : core::moduleManager.instances) {
            if (instance.module == mod) { 
                name = instname;
                break;
            }
        }
    }
    void initialize() {
        _initialized = (gpioInitialise() < 0);
    }

    void terminate() {
        gpioTerminate();
    }

    int setMode(unsigned int gpio, const unsigned int mode) {
        return gpioSetMode(gpio, mode);
    }

    int write(unsigned int gpio, unsigned int level) {
        return gpioWrite(gpio, level);
    }

    bool callInterface(const std::string& name, int code) {
        return core::modComManager.callInterface(GPIO_INFO::getInstance().name, code, (void*)&name, nullptr);
    }
};

enum {
    GPIO_IFACE_CMD_START,
    GPIO_IFACE_CMD_STOP
};
