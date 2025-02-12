#pragma once

#include <module.h>

#if defined (__PIGPIO__)
// https://pinout.xyz/
// http://abyz.me.uk/rpi/pigpio/pdif2.html
#include <pigpiod_if2.h>
#else
#define PI_INPUT 0
#define PI_OUTPUT 1
#define PI_ALT0 2
#define PI_ALT1 3
#define PI_ALT2 4
#define PI_ALT3 5
#define PI_ALT4 6
#define PI_ALT5 7
inline int pigpio_start(const char* addrStr, const char* portStr) { return -1; }
inline void pigpio_stop(int pi) {}
inline int get_mode(int pi, unsigned int gpio) { return -1; }
inline int set_mode(int pi, unsigned int gpio, const unsigned int mode) { return -1; }
inline int gpio_read(int pi, unsigned int gpio) { return -1; }
inline int gpio_write(int pi, unsigned int gpio, unsigned int level) { return -1; }
#endif

class GPIO_INFO {
public:
    enum KEYS {
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9
    };

public:
    std::string name {"undefined"};
    int deviceId = 0;

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

    int start(const char* addrStr, const char* portStr) {
        // addrStr: specifies the host or IP address of the Pi running the
        //         pigpio daemon.  It may be NULL in which case localhost
        //         is used unless overridden by the PIGPIO_ADDR environment
        //         variable.

        // portStr: specifies the port address used by the Pi running the
        //         pigpio daemon.  It may be NULL in which case "8888"
        //         is used unless overridden by the PIGPIO_PORT environment
        //         variable.
        return deviceId = pigpio_start(addrStr, portStr);
    }

    void stop() {
        // pi: >=0 (as returned by pigpio_start).
        pigpio_stop(deviceId);
    }

    int getMode(unsigned int gpio) {
        // pi: >=0 (as returned by pigpio_start).
        // gpio: 0-53.
        return get_mode(deviceId, gpio);
    }

    int setMode(unsigned int gpio, const unsigned int mode) {
        // pi: >=0 (as returned by pigpio_start).
        // gpio: 0-53.
        // mode: PI_INPUT, PI_OUTPUT, PI_ALT0, PI_ALT1,
        //     PI_ALT2, PI_ALT3, PI_ALT4, PI_ALT5.
        return set_mode(deviceId, gpio, mode);
    }

    int read(unsigned int gpio) {
        // pi: >=0 (as returned by pigpio_start).
        // gpio:0-53.
        return gpio_read(deviceId, gpio);
    }

    int write(unsigned int gpio, unsigned int level) {
        // pi: >=0 (as returned by pigpio_start).
        // gpio: 0-53.
        // level: 0, 1.
        return gpio_write(deviceId, gpio, level);
    }

    bool callInterface(const std::string& name, int code) {
        return core::modComManager.callInterface(GPIO_INFO::getInstance().name, code, (void*)&name, nullptr);
    }
};

