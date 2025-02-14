
// AS5048A = AS5048 with SPI interface
// 

// The AS5048 is a magnetic Hall sensor system manufactured in a CMOS process. A lateral Hall sensor array is used to
// measure the magnetic field components perpendicular to the surface of the chip.The AS5048 is uses self - calibration
// methods to eliminate signal offset and sensitivity drifts.
//
//  + SPI MODE = 1
// The 16 bit SPI Interface enables read / write access to the register blocks and is compatible to a standard micro
// controller interface. The SPI is active as soon as CSn is pulled low. The AS5048A then reads the digital value on the
// MOSI(master out slave in) input with every falling edge of CLK and writes on its MISO(master in slave out) output with 
// the rising edge. After 16 clock cycles CSn has to be set back to a high status in order to reset some parts of the interface core.
//

// Pure read mode: write 0xFF 0xFF and meanwhile read the result.
//



// register access mode:
//


