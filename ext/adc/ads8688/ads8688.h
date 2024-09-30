/**
 * @file ads8688.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */
 

// Command List

#define ADS8688_CMD_CONTINUE_OPERATION

typedef struct _tag_ads8688_t
{
    /**
     * @brief SPI interface which is attached to ADS8688
     */
    spi_gt *spi;

    /**
     * @brief nCS (Chip Select) GPIO interface
     */
    gpio_gt *ncs;

    /**
     * @brief nRST (Reset) & nPD (Power Done) GPIO interface
     */
    gpio_gt *nrst;

    /**
     * @brief 
     */

}adc_ads8688_t;
