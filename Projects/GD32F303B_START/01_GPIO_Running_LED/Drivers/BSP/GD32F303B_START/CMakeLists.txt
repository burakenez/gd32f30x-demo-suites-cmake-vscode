project(GD32F303B_START LANGUAGES C CXX ASM)

add_library(GD32F303B_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32F303B_START/gd32f303b_start.c
    )

target_include_directories(GD32F303B_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F303B_START
    )

target_link_libraries(GD32F303B_START PUBLIC GD32F30x_standard_peripheral)
