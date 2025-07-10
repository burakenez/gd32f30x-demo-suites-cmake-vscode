project(GD32F303B_EVAL LANGUAGES C CXX ASM)

add_library(GD32F303B_EVAL OBJECT
    ${DRIVERS_DIR}/BSP/GD32F303B_EVAL/gd32f303b_eval.c
    )

target_include_directories(GD32F303B_EVAL PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F303B_EVAL
    )

target_link_libraries(GD32F303B_EVAL PUBLIC GD32F30x_standard_peripheral)
