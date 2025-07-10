project(GD32F30x_standard_peripheral LANGUAGES C CXX ASM)

# Comment-out unused source files.
add_library(GD32F30x_standard_peripheral OBJECT
	${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_adc.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_bkp.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_can.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_crc.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_ctc.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_dac.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_dbg.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_dma.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_enet.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_exmc.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_exti.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_fmc.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_fwdgt.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_gpio.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_i2c.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_misc.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_pmu.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_rcu.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_rtc.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_sdio.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_spi.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_timer.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_usart.c
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Source/gd32f30x_wwdgt.c
    )

target_include_directories(GD32F30x_standard_peripheral PUBLIC
    ${DRIVERS_DIR}/GD32F30x_standard_peripheral/Include
    )

# CMSIS header only library is linked.
target_link_libraries(GD32F30x_standard_peripheral PUBLIC CMSIS)
