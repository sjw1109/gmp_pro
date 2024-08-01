

#ifndef _FILE_GPIO_MODEL_STM32_H_
#define _FILE_GPIO_MODEL_STM32_H_


#ifdef HAL_GPIO_MODULE_ENABLED

// STM32 GPIO MODEL
typedef struct _tag_gpio_model_stm32_t
{
	// GPIO port of STM32
	//
	GPIO_TypeDef* gpio_port;
	
	// GPIO pin of STM32
	//
	uint32_t gpio_pin;

}gpio_model_stm32_t;


// specify the GPIO model
//
#define GMP_PORT_HGPIO_T gpio_model_stm32_t


#endif // HAL_GPIO_MODULE_ENABLED

#endif // _FILE_GPIO_MODEL_STM32_H_
