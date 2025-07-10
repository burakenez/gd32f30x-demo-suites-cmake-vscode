project(GD32F303C_START LANGUAGES C CXX ASM)

add_library(GD32F303C_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32F303C_START/gd32f303c_start.c
    )

target_include_directories(GD32F303C_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F303C_START
    )

target_link_libraries(GD32F303C_START PUBLIC GD32F30x_standard_peripheral)
