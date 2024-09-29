

#ifndef _FILE_GPIO_MODEL_STM32_H_
#define _FILE_GPIO_MODEL_STM32_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifdef HAL_GPIO_MODULE_ENABLED

    // STM32 GPIO MODEL
    typedef struct _tag_gpio_model_stm32_t
    {
        // GPIO port of STM32
        //
        GPIO_TypeDef *gpio_port;

        // GPIO pin of STM32
        //
        uint32_t gpio_pin;

    } gpio_model_stm32_t;

// specify the GPIO model to be STM32 model
#define GMP_PORT_HGPIO_T gpio_model_stm32_t

    /**
     * @brief Set mode of gpio port, mode 0: input, mode 1: output.
     * @param hgpio handle of gpio. Type of GPIO handle is given by CSP.
     * @param mode target mode of GPIO. mode 0 is input mode, 1 is output mode.
     */
    void gmp_gpio_set_mode(gpio_model_stm32_t *hgpio, fast_gt mode);

    /**
     * @brief Write GPIO port. This port must be an output port.
     * Or, undefined things may happen.
     * @param hgpio handle of GPIO
     * @param level target electrical level of GPIO port.
     */
    void gmp_gpio_write(gpio_model_stm32_t *hgpio, fast_gt level);

    /**
     * @brief Read GPIO port, This port should be an input port.
     * Or the return value is undefined.
     * @param hgpio handle of GPIO
     * @return fast_gt return GPIO electrical level
     */
    fast_gt gmp_gpio_read(gpio_model_stm32_t *hgpio);

    /**
     * @brief Set GPIO electrical level to high.
     * if GPIO mode is not output mode, the result is undefined.
     * @param hgpio handle of GPIO
     */
    void gmp_gpio_set(gpio_model_stm32_t *hgpio);

    /**
     * @brief Set GPIO electrical level to low.
     * if GPIO mode is not output mode, the result is undefined.
     * @param hgpio handle of GPIO
     */
    void gmp_gpio_clear(gpio_model_stm32_t *hgpio);

#endif // HAL_GPIO_MODULE_ENABLED

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_GPIO_MODEL_STM32_H_
