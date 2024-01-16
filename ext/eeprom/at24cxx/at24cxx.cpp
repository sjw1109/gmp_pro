
// GMP library support
#include <core/gmp_core.hpp>

// extension header
#include <ext/eeprom/at24cxx/at24cxx.h>

gmp_size_t at24cxx::read(addr_type addr, at24cxx::data_type* data, gmp_size_t length)
{
    uint8_t addr_length = iic->get_reg_addr_length();
    iic->set_reg_addr_length(2);
    gmp_size_t result = iic->read(dev_addr | ((addr >> 8) & higher_mask), addr & 0xFFFF, (gmp_data_t*)data, length);
    iic->set_reg_addr_length(addr_length); // recover context
    return result;
}

gmp_size_t at24cxx::write(at24cxx::addr_type addr, const at24cxx::data_type* data, gmp_size_t length)
{
    uint8_t addr_length = iic->get_reg_addr_length();
    iic->set_reg_addr_length(2);
    gmp_size_t result = iic->write(dev_addr | ((addr >> 8) & higher_mask), addr & 0xFFFF, (gmp_data_t*)data, length);
    iic->set_reg_addr_length(addr_length); // recover context
    return result;
}

at24cxx::cell_type at24cxx::read(at24cxx::addr_type addr)
{
    cell_type result = 0;
    uint8_t addr_length = iic->get_reg_addr_length();
    iic->set_reg_addr_length(2);
    iic->read(dev_addr | ((addr >> 8) & higher_mask), addr & 0xFFFF, (gmp_data_t*)&result, reg_size);
    iic->set_reg_addr_length(addr_length); // recover context
    return result;
}

gmp_size_t at24cxx::write(at24cxx::addr_type addr, const at24cxx::cell_type data)
{
    uint8_t addr_length = iic->get_reg_addr_length();
    iic->set_reg_addr_length(2);
    gmp_size_t result = iic->write(dev_addr | ((addr >> 8) & higher_mask), addr & 0xFFFF, (const gmp_data_t*)&data, reg_size);
    iic->set_reg_addr_length(addr_length); // recover context
    return result;
}
