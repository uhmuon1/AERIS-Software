# AERIS-Software

export PICO_SDK_PATH=/home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/

** In project dir**
mkdir build/ && cd build/ && cmake .. -DCMAKE_BUILD_TYPE=Debug -DPICO_BOARD=sparkfun_thingplus && make

minicom -b 115200 -o -D /dev/ttyACM0

exit minicom - Ctrl A -> x -> enter
