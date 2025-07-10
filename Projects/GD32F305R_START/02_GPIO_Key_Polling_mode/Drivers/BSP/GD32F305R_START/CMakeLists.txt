project(GD32F305R_START LANGUAGES C CXX ASM)

add_library(GD32F305R_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32F305R_START/gd32f305r_start.c
    )

target_include_directories(GD32F305R_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F305R_START
    )

target_link_libraries(GD32F305R_START PUBLIC GD32F30x_standard_peripheral)
