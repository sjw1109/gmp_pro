
#ifndef _FILE_ARCH_C28X_H_
#define _FILE_ARCH_C28X_H_

#define ARCH_NAME "C28x"

// 2 bytes for each sizeof() result.
#define SIZEOF_UNIT (2)

// C28x BASIC DATA TYPE
#define GMP_PORT_DATA_T              int16_t
#define GMP_PORT_DATA_SIZE_PER_BITS  (16)
#define GMP_PORT_DATA_SIZE_PER_BYTES (2)

// FAST TYPES

#define GMP_PORT_FAST8_T               int16_t
#define GMP_PORT_FAST8_SIZE_PER_BITS   (16)
#define GMP_PORT_FAST8_SIZE_PER_BYTES  (2)

#define GMP_PORT_FAST16_T              int16_t
#define GMP_PORT_FAST16_SIZE_PER_BITS  (16)
#define GMP_PORT_FAST16_SIZE_PER_BYTES (2)

// ....................................................................//
// basic element data type
// This type is smallest unit of the chip
// generally, it's a 8-bit number.
//
#ifndef GMP_PORT_DATA_T
#define GMP_PORT_DATA_T               uint16_t
#define GMP_PORT_DATA_SIZE_PER_BITS   (16)
#define GMP_PORT_DATA_SIZE_PER_BYTES  (2)
#endif // GMP_PORT_DATA_T

// ....................................................................//
// basic element data type which is fast one
// This type is determined by the width of the chip data bus.
//
#ifndef GMP_PORT_FAST_T
#define GMP_PORT_FAST_T              int_fast16_t
#define GMP_PORT_FAST_SIZE_PER_BITS  (16)
#define GMP_PORT_FAST_SIZE_PER_BYTES (2)
#endif // GMP_PORT_FAST_T

#endif // _FILE_ARCH_C28X_H_
