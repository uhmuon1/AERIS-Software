# AERIS-Software

export PICO_SDK_PATH=/home/**Path**/**To/**SDK**/

** In project dir**
mkdir build/ && cd build/ && cmake .. -DCMAKE_BUILD_TYPE=Debug -DPICO_BOARD=sparkfun_thingplus

minicom -b 115200 -o -D /dev/ttyACM0

exit minicom - Ctrl A -> x -> enter
