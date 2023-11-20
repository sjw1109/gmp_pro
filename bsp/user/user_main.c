
#include <bsp/bsp_header.h>

// global variables here
uint8_t pData[2] = {0};
gmp_length_t ret;

GMP_MEM_ALIGN uint8_t test_mem[1024];

// initialize routine here.
void user_init(void)
{
	default_mem_heap = gmp_mem_setup(test_mem, 1024, GMP_MEM_BLOCK_SIZE);

	int* pint = gmp_malloc(10 * sizeof(int));
	float* pf = gmp_malloc(100 * sizeof(float));

	*pint = 10;
	*pf = 2.34;

	gmp_free(pint);
	gmp_free(pf);


}

// endless loop function here
void user_loop(void)
{
	uint8_t cmd[2] = { 0xF4, 0x2E };
	uint32_t ret = gmp_dev_write(&gmp_iic_bmp180, cmd, 2);
	HAL_Delay(5);
	//	
	//	uin8_t cmd = 0xF6;
	gmp_dev_request_read_mem(&gmp_iic_bmp180, 0xF6, 1);
	gmp_dev_read(&gmp_iic_bmp180, pData, 2);

	//	HAL_I2C_Mem_Read(&hi2c1, 0xEF, 0xF6, 1, pData, 2, 100);

	uint8_t a = 1;

	HAL_Delay(1000);


}

