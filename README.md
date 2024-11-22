# AERIS-Software

export PICO_SDK_PATH=/home/**Path**/**To/**SDK**/

** In project dir**
mkdir build/ && cd build/ && cmake .. -DPICO_BOARD=pico

minicom -b 115200 -o -D /dev/ttyACM0

exit minicom - Ctrl A -> x -> enter
