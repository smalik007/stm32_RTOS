# **Initial Setup for eclipse**
**Uncheck Exclude resource from build on following folders**
*CMSIS, Config, startup, Third-Party, StdPeriph_Deriver, inc*

**Include Paths**
**Go to project properties -> c/c++ Build -> Settings -> Tool Settings -> MCU GCC Compiler -> Includes**
Add using + button and choose workspace and navigate one by one to include
*CMSIS/core*
*CMSIS/device*
*Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F*
*Third-Party/FreeRTOS/org/Source/include*
*inc*
*Config*
*StdPeriph_Driver/inc*

**Preprocessors**
**Go to project properties -> c/c++ Build -> Settings -> Tool Settings -> MCU GCC Compiler -> Preprocessor**
Add using + button
*STM32F446xx*
*NUCLEO_F446RE*
*USE_STDPERIPH_DRIVER*



