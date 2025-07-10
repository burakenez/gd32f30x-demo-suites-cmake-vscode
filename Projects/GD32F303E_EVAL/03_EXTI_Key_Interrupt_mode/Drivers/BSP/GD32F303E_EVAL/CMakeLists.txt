project(GD32F303E_EVAL LANGUAGES C CXX ASM)

add_library(GD32F303E_EVAL OBJECT
    ${DRIVERS_DIR}/BSP/GD32F303E_EVAL/gd32f303e_eval.c
    )

target_include_directories(GD32F303E_EVAL PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F303E_EVAL
    )

target_link_libraries(GD32F303E_EVAL PUBLIC GD32F30x_standard_peripheral)
