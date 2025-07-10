project(GD32F303R_START LANGUAGES C CXX ASM)

add_library(GD32F303R_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32F303R_START/gd32f303r_start.c
    )

target_include_directories(GD32F303R_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F303R_START
    )

target_link_libraries(GD32F303R_START PUBLIC GD32F30x_standard_peripheral)
