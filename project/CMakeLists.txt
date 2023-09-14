cmake_minimum_required(VERSION 3.27)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

project(template C ASM)
set(ENABLE_BUILD_HEX TRUE)
set(ENABLE_BUILD_BIN TRUE)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(KEIL_PATH D:/Code/Keil_v5)
set(ARMCC_PATH ${KEIL_PATH}/ARM/ARMCC/bin)
set(ARMCLANG_PATH ${KEIL_PATH}/ARM/ARMCLANG/bin)

set(CMAKE_SYSTEM_PROCESSOR Cortex-M3)
add_compile_definitions(STM32F10X_HD USE_STDPERIPH_DRIVER debug=1 __MICROLIB)

include_directories(
        ../user
        ../user/api
        ../user/api/MQTT
        ../user/api/HMAC
        ../STM32F10x_StdPeriph_Driver/inc
        ../ld3320
        ../FatFs/Fatfs_src
        ../sdCard_spi
        D:/Code/Keil_v5/ARM/Packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include
        D:/Code/Keil_v5/ARM/Packs/Keil/STM32F1xx_DFP/2.4.1/Device/Include
)

file(GLOB SOURCES
        ../startup/startup_stm32f10x_hd.s
        ../STM32F10x_StdPeriph_Driver/src/*.c
        ../user/*.c
        ../user/api/*.c
        ../user/api/HMAC/*.c
        ../user/api/MQTT/*.c
)

MESSAGE("\n========== ARMClang COMPILER ==========\n")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 03)

set(CMAKE_C_COMPILER ${ARMCLANG_PATH}/armclang)
set(CMAKE_CXX_COMPILER ${ARMCLANG_PATH}/armclang)
set(CMAKE_ASM_COMPILER ${ARMCLANG_PATH}/armclang)
set(CMAKE_OBJCOPY ${ARMCLANG_PATH}/fromelf)

set(CMAKE_C_COMPILER_AR ${ARMCLANG_PATH}/armar)
set(CMAKE_CXX_COMPILER_AR ${ARMCLANG_PATH}/armar)
set(CMAKE_ASM_COMPILER_AR ${ARMCLANG_PATH}/armar)

set(C_CXX_COMPILE_OPTIONS -xc -std=c99 --target=arm-arm-none-eabi -mcpu=cortex-m3 -c
        -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
        -D__MICROLIB -gdwarf-4 -O0 -ffunction-sections
        -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes
        -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path
        -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command
        -Wno-documentation -Wno-license-management -Wno-parentheses-equality -Wno-reserved-identifier
        -Wno-invalid-source-encoding
        )

set(ASM_COMPILE_OPTIONS --target=arm-arm-none-eabi -mcpu=cortex-m3 -masm=armasm  -Wa,armasm,--diag_suppress=A1950W -c -gdwarf-4)

add_compile_options(
        "$<$<COMPILE_LANGUAGE:C>:${C_CXX_COMPILE_OPTIONS}>"
        "$<$<COMPILE_LANGUAGE:ASM>:${ASM_COMPILE_OPTIONS}>"
)

include_directories(${ARMCLANG_PATH}/../include)

set(LINK_EXE ${ARMCLANG_PATH}/armlink)
set(LINK_SCT_FILE ../project/Objects/stm32.sct)

set(CMAKE_C_LINK_EXECUTABLE "${LINK_EXE} \
        --cpu Cortex-M3 \
        <OBJECTS> \
        <CMAKE_C_LINK_FLAGS> \
        --library_type=microlib --strict --scatter ${LINK_SCT_FILE} \
        --summary_stderr --info summarysizes --map --load_addr_map_info --xref --callgraph --symbols \
        --info sizes --info totals --info unused --info veneers \
        <LINK_LIBRARIES> \
        --list <TARGET_BASE>.map \
        -o <TARGET> \
")

add_executable(${PROJECT_NAME}.axf ${SOURCES})

if (ENABLE_BUILD_HEX)
    set(FW_hex ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
    add_custom_command(TARGET ${PROJECT_NAME}.axf POST_BUILD
            COMMENT "Building ${FW_hex}"
            COMMAND ${CMAKE_OBJCOPY} --i32 $<TARGET_FILE:${PROJECT_NAME}.axf> --output=${FW_hex}
            )
endif ()

if (ENABLE_BUILD_BIN)
    set(FW_bin ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
    add_custom_command(TARGET ${PROJECT_NAME}.axf POST_BUILD
            COMMENT "Building ${FW_bin}"
            COMMAND ${CMAKE_OBJCOPY} --bin $<TARGET_FILE:${PROJECT_NAME}.axf> --output=${FW_bin}
            )
endif ()


