set(CORE_NAME "M7" CACHE STRING "" FORCE)
set(HAS_MIKROBUS "true" CACHE STRING "" FORCE)
set(MCU_NAME "STM32F745VG" CACHE STRING "" FORCE)
set(_MSDK_BOARD_NAME_ "BOARD_UNI_DS_V8" CACHE STRING "" FORCE)
set(_MSDK_DIP_SOCKET_TYPE_ "" CACHE STRING "" FORCE)
set(_MSDK_ETH_PHY_CHIP_ "NULL" CACHE STRING "" FORCE)
set(_MSDK_HAL_LOW_LEVEL_TARGET_ "mikroe" CACHE STRING "" FORCE)
set(_MSDK_MCU_CARD_NAME_ "MCU_CARD_31_FOR_STM32" CACHE STRING "" FORCE)
set(_MSDK_PACKAGE_ID_ "LQFP" CACHE STRING "" FORCE)
set(_MSDK_PACKAGE_NAME_ "Tx" CACHE STRING "" FORCE)
set(_MSDK_PACKAGE_PIN_COUNT_ "100" CACHE STRING "" FORCE)
set(_MSDK_SHIELD_ "" CACHE STRING "" FORCE)
set(_MSDK_TFT_BOARD_ "TFT_BOARD_4_RESISTIVE" CACHE STRING "" FORCE)
set(_MSDK_TFT_HEIGHT_ "272" CACHE STRING "" FORCE)
set(_MSDK_TFT_TP_ "__TP_STMPE811__" CACHE STRING "" FORCE)
set(_MSDK_TFT_TP_CONTROLLER_ "_DEFAULT_" CACHE STRING "" FORCE)
set(_MSDK_TFT_TP_ROTATE_ "TP_ROTATE_180" CACHE STRING "" FORCE)
set(_MSDK_TFT_TYPE_ "__TFT_RESISTIVE__" CACHE STRING "" FORCE)
set(_MSDK_TFT_WIDTH_ "480" CACHE STRING "" FORCE)
set(flatten_level "FLATTEN_ME_LEVEL_HIGH" CACHE STRING "" FORCE)
set(standard_output "Application output" CACHE STRING "" FORCE)
set(TOOLCHAIN_ID "gcc_arm_none_eabi" CACHE STRING "" FORCE)
set(OSC "216" CACHE STRING "" FORCE)

# set CMAKE_SYSTEM_NAME to define build as CMAKE_CROSSCOMPILING
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# specify cross compilers and tools
set(CMAKE_C_COMPILER "C:/Users/klasn/AppData/Local/MIKROE/NECTOStudio7/packages/compilers/gcc/arm/gcc-arm-none-eabi/bin/arm-none-eabi-gcc.exe" CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER "C:/Users/klasn/AppData/Local/MIKROE/NECTOStudio7/packages/compilers/gcc/arm/gcc-arm-none-eabi/bin/arm-none-eabi-g++.exe" CACHE INTERNAL "")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

file(TO_CMAKE_PATH "D:/Dokumenti/Necto-Studio/Projects/FT800 Demo/.meproject/setup/Debug/lib/cmake" MIKROSDK_PATH)
file(TO_CMAKE_PATH "D:/Dokumenti/Necto-Studio/Projects/FT800 Demo/.meproject/setup/Debug/lib/cmake" MIKROC_CORE_PATH)

set(MIKROSDK_ROOT_PATH ${MIKROSDK_PATH})
set(MIKROC_CORE_ROOT_PATH ${MIKROC_CORE_PATH})
#append to cmake_prefix_path
list(APPEND CMAKE_PREFIX_PATH ${MIKROSDK_ROOT_PATH})
list(APPEND CMAKE_PREFIX_PATH ${MIKROC_CORE_ROOT_PATH})

list(APPEND CMAKE_MODULE_PATH "C:/Users/klasn/AppData/Local/MIKROE/NECTOStudio7/cmake;C:/Users/klasn/AppData/Local/MIKROE/NECTOStudio7/packages/core/ARM/gcc_clang/arm_gcc_clang_stm32f7x/cmake;C:/Users/klasn/AppData/Local/MIKROE/NECTOStudio7/packages/mikroe_utils_common;D:/Dokumenti/Necto-Studio/Projects/FT800 Demo/.meproject/setup/Debug/lib/cmake;")

if (DEFINED CORE_NAME AND NOT "${CORE_NAME}" STREQUAL "")
    include(coreUtils)
    set_flags(FLAGS)
    message(INFO ": ${FLAGS}")

    # add compiler option flags
    add_compile_options(${FLAGS})
    # add link option flags
    add_link_options(${FLAGS})
endif()



if (SDK_SETUP_BUILD)
    add_link_options(-T "%LINKER_SCRIPT%")
endif()
