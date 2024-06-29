
#include <windows.h>

#include <core/gmp_core.hpp>

// iic port
//GMP_LENGTH_T gmp_iic_transmit(
//    GMP_PORT_IIC_HANDLE_T hi2c,
//    GMP_LENGTH_T iic_dev_addr,
//    GMP_DATA_ELEMENT_T* data,
//    GMP_LENGTH_T len)
//{
//
//    return 0;
//}
//
//void gmp_iic_request_read_mem
//(
//    GMP_PORT_IIC_HANDLE_T hi2c,
//    GMP_CMD_T cmd,
//    GMP_LENGTH_T length
//)
//{
//    return;
//
//}
//
//GMP_LENGTH_T gmp_iic_receive(
//    GMP_PORT_IIC_HANDLE_T hi2c,
//    GMP_LENGTH_T iic_dev_addr,
//    GMP_CMD_T cmd, GMP_LENGTH_T cmd_len,
//    GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len)
//{
//    return len;
//}

time_gt gmp_port_system_tick()
{
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    return sys.wMinute * 60 + sys.wSecond;
}

void gmp_port_system_stuck(
    void
)
{

}

#if defined SPECIFY_ENABLE_FEED_WATCHDOG
// This function should be implemented by user,
// Every Loop routine, this function would be called.
// And user should ensure that the function has only one thing is to feed the watchdog
void gmp_port_feed_dog(
    void
)
{

}
#endif // SPECIFY_ENABLE_FEED_WATCHDOG

void gmp_setup_peripheral()
{}

void gmp_csp_startup(void)
{

}