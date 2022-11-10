# https://github.com/AlexandreRouma/SDRPlusPlus
# https://raspberrypi.stackexchange.com/questions/110910/pigpio-without-sudo
# https://raspberrypi.stackexchange.com/questions/66890/accessing-pwm-module-without-root-permissions
# http://abyz.me.uk/rpi/pigpio/index.html

mkdir -p build
cd build

# Release build
cmake -DCMAKE_BUILD_TYPE=Release -DOPT_BUILD_GPIO=ON ..
cmake --build . --config Release -j 4 -- 
sudo cmake --build . --config Release --target install --
sudo cmake --build . --config Release --target uninstall --

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug -DOPT_BUILD_GPIO=ON ..
cmake --build . --config Debug -j 4 -- 
sudo cmake --build . --config Debug --target install --
sudo cmake --build . --config Debug --target uninstall --

# Then, you will need to edit the root_dev/config.json file to point to the modules that were built:

# For debug:
# ./usr/lib/aarch64-linux-gnu/libasan.so.6
# ./usr/lib/aarch64-linux-gnu/libasan.so.6.0.0
# ./usr/lib/gcc/aarch64-linux-gnu/10/libasan.so

export LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libasan.so.6
# export LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libasan.so.6.0.0
# export LD_PRELOAD=/usr/lib/gcc/aarch64-linux-gnu/10/libasan.so


# ...
# "modules": [
#     "./build/radio/Release/radio.dll",
#     "./build/recorder/Release/recorder.dll",
#     "./build/rtl_tcp_source/Release/rtl_tcp_source.dll",
#     "./build/soapy_source/Release/soapy_source.dll",
#     "./build/audio_sink/Release/audio_sink.dll"
# ]
# ...

# ...
# "modulesDirectory": "./root_dev/modules",
# ...
# "resourcesDirectory": "./root_dev/res",
# ...

./sdrpp -r ../root_dev
