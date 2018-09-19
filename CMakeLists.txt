INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)


# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})


SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFRDM_K66F")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-unused-local-typedefs")



SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DFRDM_K66F")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-write-strings")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-function")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-missing-field-initializers")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-local-typedefs")



SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MK66FN2M0VMD18")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFRDM_K66F")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFREEDOM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-local-typedefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -specs=nosys.specs")

include_directories(${ProjDirPath}/board/)
include_directories(${ProjDirPath}/k66f-reqs/CMSIS/)
include_directories(${ProjDirPath}/k66f-reqs/drivers)
include_directories(${ProjDirPath}/k66f-reqs/freertos/Source)
include_directories(${ProjDirPath}/k66f-reqs/freertos/Source/include)
include_directories(${ProjDirPath}/k66f-reqs/freertos/Source/portable/GCC/ARM_CM4F)
include_directories(${ProjDirPath}/source/)
include_directories(${ProjDirPath}/spi_proto/src)
include_directories(${ProjDirPath}/k66f-reqs/startup/)
include_directories(${ProjDirPath}/k66f-reqs/utilities/)


set(BOARD_CFG
"${ProjDirPath}/board/board.c"
"${ProjDirPath}/board/board.h"
"${ProjDirPath}/board/clock_config.c"
"${ProjDirPath}/board/clock_config.h"
"${ProjDirPath}/board/pin_mux.c"
"${ProjDirPath}/board/pin_mux.h"
)

set(FSL_DRIVERS
"${ProjDirPath}/k66f-reqs/drivers/fsl_adc16.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_adc16.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_clock.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_clock.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_common.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_common.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dac.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dac.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dmamux.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dmamux.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dspi.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dspi.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dspi_edma.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dspi_edma.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dspi_freertos.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_dspi_freertos.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_edma.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_edma.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_gpio.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_gpio.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_i2c.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_i2c.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_i2c_freertos.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_i2c_freertos.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_lpuart.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_lpuart.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_port.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_smc.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_smc.h"
"${ProjDirPath}/k66f-reqs/drivers/fsl_uart.c"
"${ProjDirPath}/k66f-reqs/drivers/fsl_uart.h"
)

set(FREERTOS_SOURCE
"${ProjDirPath}/k66f-reqs/freertos/Source/croutine.c"
"${ProjDirPath}/k66f-reqs/freertos/Source/event_groups.c"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/FreeRTOS.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/StackMacros.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/croutine.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/event_groups.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/list.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/mpu_wrappers.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/portable.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/projdefs.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/queue.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/semphr.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/task.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/include/timers.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/list.c"
"${ProjDirPath}/k66f-reqs/freertos/Source/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/k66f-reqs/freertos/Source/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/k66f-reqs/freertos/Source/portable/MemMang/heap_4.c"
"${ProjDirPath}/k66f-reqs/freertos/Source/queue.c"
"${ProjDirPath}/k66f-reqs/freertos/Source/tasks.c"
"${ProjDirPath}/k66f-reqs/freertos/Source/timers.c"
)

set(K66F_STARTUP
"${ProjDirPath}/k66f-reqs/startup/startup_MK66F18.S"
"${ProjDirPath}/k66f-reqs/startup/system_MK66F18.c"
"${ProjDirPath}/k66f-reqs/startup/system_MK66F18.h"
)

set(SPI_PROTO_SOURCES
"${ProjDirPath}/spi_proto/src/crc16.c"
"${ProjDirPath}/spi_proto/src/crc16.h"
"${ProjDirPath}/source/spi_edma_task.cpp"
"${ProjDirPath}/source/spi_edma_task.h"
"${ProjDirPath}/spi_proto/src/spi_proto.c"
"${ProjDirPath}/spi_proto/src/spi_chunks.cpp"
"${ProjDirPath}/spi_proto/src/spi_proto.h"
"${ProjDirPath}/source/spi_proto_slave.cpp"
"${ProjDirPath}/source/spi_proto_slave.h"
"${ProjDirPath}/source/spi_chunks_slave.cpp"
"${ProjDirPath}/source/spi_chunks_slave.h"
)

add_executable(amm-tiny.elf
${BOARD_CFG}
${FSL_DRIVERS}
${FREERTOS_SOURCE}
${SPI_PROTO_SOURCES}
${K66F_STARTUP}
"${ProjDirPath}/source/FreeRTOSConfig.h"
"${ProjDirPath}/source/action.cpp"
"${ProjDirPath}/source/action.h"
"${ProjDirPath}/source/ammdk-carrier/carrier_gpio.cpp"
"${ProjDirPath}/source/ammdk-carrier/carrier_gpio.h"
"${ProjDirPath}/source/ammdk-carrier/dac.cpp"
"${ProjDirPath}/source/ammdk-carrier/dac.h"
"${ProjDirPath}/source/ammdk-carrier/maxon.cpp"
"${ProjDirPath}/source/ammdk-carrier/maxon.h"
"${ProjDirPath}/source/ammdk-carrier/solenoid.cpp"
"${ProjDirPath}/source/ammdk-carrier/solenoid.h"
"${ProjDirPath}/source/button.cpp"
"${ProjDirPath}/source/button.h"
"${ProjDirPath}/source/controllers/adc.cpp"
"${ProjDirPath}/source/controllers/adc.h"
"${ProjDirPath}/source/controllers/dac.cpp"
"${ProjDirPath}/source/controllers/dac.h"
"${ProjDirPath}/source/controllers/gpio.cpp"
"${ProjDirPath}/source/controllers/gpio.h"
"${ProjDirPath}/source/flowsensor.cpp"
"${ProjDirPath}/source/flowsensor.h"
"${ProjDirPath}/source/amm-tiny.cpp"
"${ProjDirPath}/source/maxon.cpp"
"${ProjDirPath}/source/maxon.h"
"${ProjDirPath}/source/mule-1/air_tank.cpp"
"${ProjDirPath}/source/mule-1/air_tank.h"
"${ProjDirPath}/source/pressure_adc.cpp"
"${ProjDirPath}/source/pressure_adc.h"
"${ProjDirPath}/source/pressuresensor.cpp"
"${ProjDirPath}/source/pressuresensor.h"
"${ProjDirPath}/source/sensirion.cpp"
"${ProjDirPath}/source/sensirion.h"
"${ProjDirPath}/source/solenoid.cpp"
"${ProjDirPath}/source/solenoid.h"
"${ProjDirPath}/k66f-reqs/utilities/fsl_debug_console.c"
"${ProjDirPath}/k66f-reqs/utilities/fsl_debug_console.h"
"${ProjDirPath}/k66f-reqs/utilities/fsl_sbrk.c"
)

add_executable(fluidmanager.elf
${BOARD_CFG}
${FSL_DRIVERS}
${FREERTOS_SOURCE}
${SPI_PROTO_SOURCES}
${K66F_STARTUP}
"${ProjDirPath}/source/FreeRTOSConfig.h"
"${ProjDirPath}/source/ammdk-carrier/carrier_gpio.cpp"
"${ProjDirPath}/source/ammdk-carrier/carrier_gpio.h"
"${ProjDirPath}/source/ammdk-carrier/maxon.cpp"
"${ProjDirPath}/source/ammdk-carrier/maxon.h"
"${ProjDirPath}/source/ammdk-carrier/solenoid.cpp"
"${ProjDirPath}/source/ammdk-carrier/solenoid.h"
"${ProjDirPath}/source/controllers/gpio.cpp"
"${ProjDirPath}/source/controllers/gpio.h"
"${ProjDirPath}/source/fluidmanager.cpp"
"${ProjDirPath}/source/maxon.cpp"
"${ProjDirPath}/source/maxon.h"
"${ProjDirPath}/source/mule-1/air_tank.cpp"
"${ProjDirPath}/source/mule-1/air_tank.h"
"${ProjDirPath}/source/pressuresensor.cpp"
"${ProjDirPath}/source/pressuresensor.h"
"${ProjDirPath}/source/solenoid.cpp"
"${ProjDirPath}/source/solenoid.h"
"${ProjDirPath}/k66f-reqs/utilities/fsl_debug_console.c"
"${ProjDirPath}/k66f-reqs/utilities/fsl_debug_console.h"
"${ProjDirPath}/k66f-reqs/utilities/fsl_sbrk.c"
)

add_executable(ivc.elf
${BOARD_CFG}
${FSL_DRIVERS}
${FREERTOS_SOURCE}
${SPI_PROTO_SOURCES}
${K66F_STARTUP}
"${ProjDirPath}/source/FreeRTOSConfig.h"
"${ProjDirPath}/source/ammdk-carrier/carrier_gpio.cpp"
"${ProjDirPath}/source/ammdk-carrier/carrier_gpio.h"
"${ProjDirPath}/source/ammdk-carrier/solenoid.cpp"
"${ProjDirPath}/source/ammdk-carrier/solenoid.h"
"${ProjDirPath}/source/controllers/gpio.cpp"
"${ProjDirPath}/source/controllers/gpio.h"
"${ProjDirPath}/source/flowsensor.cpp"
"${ProjDirPath}/source/flowsensor.h"
"${ProjDirPath}/source/ivc.cpp"
"${ProjDirPath}/source/pressuresensor.cpp"
"${ProjDirPath}/source/pressuresensor.h"
"${ProjDirPath}/source/solenoid.cpp"
"${ProjDirPath}/source/solenoid.h"
"${ProjDirPath}/k66f-reqs/utilities/fsl_debug_console.c"
"${ProjDirPath}/k66f-reqs/utilities/fsl_debug_console.h"
"${ProjDirPath}/k66f-reqs/utilities/fsl_sbrk.c"
)

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MK66FN2M0xxx18_flash.ld -static")

TARGET_LINK_LIBRARIES(amm-tiny.elf -Wl,--start-group)
target_link_libraries(amm-tiny.elf debug m)

target_link_libraries(amm-tiny.elf debug c)

target_link_libraries(amm-tiny.elf debug gcc)

target_link_libraries(amm-tiny.elf debug nosys)

target_link_libraries(amm-tiny.elf optimized m)

target_link_libraries(amm-tiny.elf optimized c)

target_link_libraries(amm-tiny.elf optimized gcc)

target_link_libraries(amm-tiny.elf optimized nosys)

TARGET_LINK_LIBRARIES(amm-tiny.elf -Wl,--end-group)

TARGET_LINK_LIBRARIES(fluidmanager.elf -Wl,--start-group)
target_link_libraries(fluidmanager.elf debug m)

target_link_libraries(fluidmanager.elf debug c)

target_link_libraries(fluidmanager.elf debug gcc)

target_link_libraries(fluidmanager.elf debug nosys)

target_link_libraries(fluidmanager.elf optimized m)

target_link_libraries(fluidmanager.elf optimized c)

target_link_libraries(fluidmanager.elf optimized gcc)

target_link_libraries(fluidmanager.elf optimized nosys)

TARGET_LINK_LIBRARIES(fluidmanager.elf -Wl,--end-group)

TARGET_LINK_LIBRARIES(ivc.elf -Wl,--start-group)
target_link_libraries(ivc.elf debug m)

target_link_libraries(ivc.elf debug c)

target_link_libraries(ivc.elf debug gcc)

target_link_libraries(ivc.elf debug nosys)

target_link_libraries(ivc.elf optimized m)

target_link_libraries(ivc.elf optimized c)

target_link_libraries(ivc.elf optimized gcc)

target_link_libraries(ivc.elf optimized nosys)

TARGET_LINK_LIBRARIES(ivc.elf -Wl,--end-group)

