cmake_minimum_required(VERSION 3.19)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(New_FT800_Demo LANGUAGES MikroC)
else()
    project(New_FT800_Demo LANGUAGES C ASM)
endif()

include(mikroeUtils)

if (${MIKROSDK_TYPE} STREQUAL "legacy")
    legacy_cmake_path()
endif()

set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)

add_executable(New_FT800_Demo
main.c
)

############################ New_FT800_Demo GENERATED CODE START ###########################
find_package(MikroSDK.Ft800 REQUIRED)
target_link_libraries(New_FT800_Demo PUBLIC MikroSDK.Ft800)
find_package(MikroSDK.Driver.GPIO.Out REQUIRED)
target_link_libraries(New_FT800_Demo PUBLIC MikroSDK.Driver.GPIO.Out)
find_package(MikroSDK.Driver REQUIRED)
target_link_libraries(New_FT800_Demo PUBLIC MikroSDK.Driver)
find_package(MikroSDK.Conversions REQUIRED)
target_link_libraries(New_FT800_Demo PUBLIC MikroSDK.Conversions)
find_package(MikroSDK.Board REQUIRED)
target_link_libraries(New_FT800_Demo PUBLIC MikroSDK.Board)
find_package(MikroC.Core REQUIRED)
target_link_libraries(New_FT800_Demo PUBLIC MikroC.Core)
############################ New_FT800_Demo GENERATED CODE END ###########################
find_package(MikroC.Core REQUIRED)
target_link_libraries(New_FT800_Demo
    PUBLIC
        MikroC.Core
)

if (${MIKROSDK_TYPE} STREQUAL "legacy")
    find_package(MikroC.System REQUIRED)
    target_link_libraries(New_FT800_Demo
        PUBLIC
            MikroC.System
    )
endif()

if(COMPILER_REVISION)
    if(${COMPILER_REVISION} VERSION_GREATER_EQUAL "1.0")
        if(${TOOLCHAIN_LANGUAGE} STREQUAL "GNU")
            if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.meproject)
                fetch_startup_name(${CMAKE_CURRENT_SOURCE_DIR}/.meproject startupName)
                if(startupName)
                    add_startup_all_targets(. ${CMAKE_CURRENT_SOURCE_DIR} ${startupName})
                endif()
                fetch_linker_name(${CMAKE_CURRENT_SOURCE_DIR}/.meproject linkerName)
                if(linkerName)
                    add_ld_all_targets(. ${CMAKE_CURRENT_SOURCE_DIR} ${linkerName})
                endif()
            endif()
        endif()
    endif()
endif()
