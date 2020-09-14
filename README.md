# **Initial Setup for eclipse**
**Uncheck Exclude resource from build on following folders**\
*CMSIS, Config, startup, Third-Party, StdPeriph_Deriver, inc*\

**Include Paths**\
**Go to project properties -> c/c++ Build -> Settings -> Tool Settings -> MCU GCC Compiler -> Includes**\
Add using + button and choose workspace and navigate one by one to include\
*CMSIS/core*\
*CMSIS/device*\
*Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F*\
*Third-Party/FreeRTOS/org/Source/include*\
*inc*\
*Config*\
*StdPeriph_Driver/inc*\

**Preprocessors**\
**Go to project properties -> c/c++ Build -> Settings -> Tool Settings -> MCU GCC Compiler ->Preprocessor**\
Add using + button\
*STM32F446xx*\
*NUCLEO_F446RE*\
*USE_STDPERIPH_DRIVER*\

**Qemu installation**\
*Pre-requisite*\
*1. Eclipse GNU MCU plugin*\
https://gnu-mcu-eclipse.github.io/plugins/download/
*2. MinGW with gdb*\ 

Download page: https://github.com/xpack-dev-tools/qemu-arm-xpack/releases/tag/v2.8.0-9/\
https://xpack.github.io/qemu-arm/install/ *check manual installtion*\
extract to C:/User/user_name/AppData/Roaming/xPacks/qemu-arm/\
check by executing $ xPacks/qemu-arm/bin/qemu-system-gnuarmeclipse.exe --version\



