project(GD32F303C_EVAL LANGUAGES C CXX ASM)

add_library(GD32F303C_EVAL OBJECT
    ${DRIVERS_DIR}/BSP/GD32F303C_EVAL/gd32f303c_eval.c
    )

target_include_directories(GD32F303C_EVAL PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F303C_EVAL
    )

target_link_libraries(GD32F303C_EVAL PUBLIC GD32F30x_standard_peripheral)
