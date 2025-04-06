# AERIS-Software

export PICO_SDK_PATH=/home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/

** In project dir**
sudo rm -rf build && mkdir build/ && cd build/ && cmake .. -DCMAKE_BUILD_TYPE=Debug -DPICO_BOARD=sparkfun_thingplus && make && cd .. && rm -f putty.log && putty

minicom -b 115200 -o -D /dev/ttyACM0

exit minicom - Ctrl A -> x -> enter
