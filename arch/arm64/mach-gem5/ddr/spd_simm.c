/*!****************************************************************************
* @file spd_simm.c
*
* @par Description:
*    This module provides the SPD simulation facilities that allow for higher
*    level code to operate as if SPD data is available from the DIMMs when the
*    HW level mechanisms do not exist.
*
* @par Notes:
*    The provided SPD_DATA_STORE [spd_base] address point to a reserved block
*    of memory large enough to hold the SPD information block as defined by
*    spd_block_size. If SPD simulation is used the amount of reserved space
*    must be doubled to contain the simulated SPD information.
*
******************************************************************************/

/******************************************************************************
* Included Headers
******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "ddr_driver.h"
#include  "mddr.h"
#include "mddr_init.h"
#include "spd_smbus.h"

/******************************************************************************
* Defined Constants
******************************************************************************/

/*!****************************************************************************
*
* @par Description
*    Special simulation specific bus address marker value used to indicate
*    the slot associated to the address is to be treated as unpopulated.
*
******************************************************************************/
#define SPD_SIMM_UNPOPULATED_MARKER                                  0x0


/******************************************************************************
* Derived data types
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/

/*!****************************************************************************
*
* @par Description
*    Flag used to globally signal that simulated SPD should be used.
*
*
* @par Notes:
*    This flag is set to "true" in spd_simm_init().
*
*    In Emulation there is no SPD HW so simulation is to be used instead.
*    In Silicon where SPD HW is present there are 2 options:
*    1. turn the simulation on by setting the flag spd_use_simm_in_sil below
*     from "false" to "true".
*    2. run on real SPD HW; in this case the flag spd_use_simm_in_sil = false
*
******************************************************************************/
bool spd_use_simulated_spd = false;

/*!****************************************************************************
*
* @par Description
*    Flag used to globally signal that simulated SPD should be used in silicon.
*
* @par Notes:
*    Set this flag to 1 here to turn on simulation in Silicon
******************************************************************************/
bool spd_use_simm_in_sil = false; // Set this flag to true to enable SPD simulation in Silicon.

/*!****************************************************************************
*
* @par Description
*    Pointer to the selected simulated SPD data array.
*
******************************************************************************/
uint8_t *spd_sim_array_ptr;

extern dimm_slot_smbus_info_t spd_smbus_info[DDR_MAX_NUM_CHANS][DDR_MAX_NUM_SLOTS_PER_CHAN];


/*!****************************************************************************
*
* @defgroup spd_sim_data SPD Simulate Data
*
* @par Description
*     SPD data arrays used to simulate various DDR DIMMs
*
* @{
******************************************************************************/

// SPD_MODULE_TYPE_DDR4_RDIMM : DDR4 RDIMM  8GB 2Rx8 2133  Micron - SOD backup RDIMM #1(slower): MCN:825-71864-0000
uint8_t spd_ddr4_2Rx8_8GB_rdimm[] =       //!< MT18ASF1G72PDZ-2G1A1 : bringup8.bin
{
    0x23, 0x10, 0x0c, 0x01, 0x84, 0x19, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x09, 0x0b, 0x80, 0x00,
    0x00, 0x00, 0x08, 0x0c, 0xf4, 0x1b, 0x00, 0x00, 0x6c, 0x6c, 0x6c, 0x11, 0x08, 0x74, 0x20, 0x08,
    0x00, 0x05, 0x70, 0x03, 0x00, 0xa8, 0x1e, 0x2b, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x2c, 0x0c, 0x2c,
    0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0xb5, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc2, 0x27, 0x3d,
    0x11, 0x11, 0x04, 0x05, 0x00, 0x86, 0x32, 0xb1, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0x45,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x2c, 0x0c, 0x14, 0x34, 0x0d, 0x0d, 0x16, 0x73, 0x31, 0x38, 0x41, 0x53, 0x46, 0x31, 0x47,
    0x37, 0x32, 0x50, 0x44, 0x5a, 0x2d, 0x32, 0x47, 0x31, 0x41, 0x31, 0x20, 0x20, 0x31, 0x80, 0x2c,
    0x41, 0x44, 0x44, 0x41, 0x42, 0x38, 0x4b, 0x57, 0x30, 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// SPD_MODULE_TYPE_DDR4_RDIMM : DDR4 RDIMM  4GB 1Rx8 2400  Micron - SOD backup RDIMM #2(fast): MCN:???
uint8_t spd_ddr4_1Rx8_4GB_rdimm[] =       //!< MTA9ASF512G72PZ-2G3A2: bringupc.bin
{
    0x23, 0x10, 0x0c, 0x01, 0x84, 0x19, 0x00, 0x08, 0x00, 0x60, 0x00, 0x03, 0x01, 0x0b, 0x80, 0x00,
    0x00, 0x00, 0x07, 0x0d, 0xf8, 0x7f, 0x00, 0x00, 0x6e, 0x6e, 0x6e, 0x11, 0x00, 0x6e, 0x20, 0x08,
    0x00, 0x05, 0x70, 0x03, 0x00, 0xa8, 0x1b, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x2e, 0x16, 0x36,
    0x16, 0x36, 0x16, 0x36, 0x0e, 0x2e, 0x23, 0x04, 0x2b, 0x0c, 0x2b, 0x0c, 0x23, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0xb5, 0x00, 0x00, 0x00, 0x00, 0xe7, 0xd6, 0xa0, 0xdf,
    0x11, 0x11, 0x03, 0x05, 0x00, 0x80, 0xb3, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0xf9,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x41, 0x53, 0x46, 0x35, 0x31, 0x32,
    0x37, 0x32, 0x50, 0x5a, 0x2d, 0x32, 0x47, 0x33, 0x41, 0x32, 0x20, 0x20, 0x20, 0x32, 0x80, 0x2c,
    0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};





// SPD_MODULE_TYPE_DDR4_RDIMM : DDR4-2400 (1200 MHz) - SOD Primary RDIMM : MCN:825-71946-0000
uint8_t spd_ddr4_1Rx4_8GB_rdimm[] =      //!< MTA18ASF1G72PZ-2G3A3 : spd_ddr4_mt_1Rx4_8GB_RDIMM.bin
{
    0x23, 0x10, 0x0c, 0x01, 0x84, 0x21, 0x00, 0x08, 0x00, 0x60, 0x00, 0x03, 0x00, 0x0b, 0x80, 0x00,
    0x00, 0x00, 0x07, 0x0d, 0xf8, 0x0f, 0x00, 0x00, 0x6e, 0x6e, 0x6e, 0x11, 0x00, 0x6e, 0x20, 0x08,
    0x00, 0x05, 0x70, 0x03, 0x00, 0x68, 0x1b, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x0d, 0x15, 0x0d,
    0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0xb5, 0x00, 0x00, 0x00, 0x00, 0xe7, 0xd6, 0x80, 0x65,
    0x11, 0x11, 0x22, 0x05, 0x00, 0x80, 0xb3, 0x30, 0x00, 0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf5, 0x72,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x38, 0x41, 0x53, 0x46, 0x31, 0x47,
    0x37, 0x32, 0x50, 0x5a, 0x2d, 0x32, 0x47, 0x33, 0x41, 0x33, 0x20, 0x20, 0x20, 0x33, 0x80, 0x2c,
    0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// SPD_MODULE_TYPE_DDR4_RDIMM : DDR4 RDIMM  8GB 2Rx8 2400 Micron - SOD backup RDIMM #2(faster):
uint8_t spd_ddr4_2Rx8_8GB_rdimm_2[] =       //!< MT18ASF1G72PDZ-2G3A2 : bringup2.bin
{
    0x23, 0x10, 0x0c, 0x01, 0x84, 0x19, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x09, 0x0b, 0x80, 0x00,
    0x00, 0x00, 0x08, 0x0c, 0xf4, 0x1b, 0x00, 0x00, 0x6c, 0x6c, 0x6c, 0x11, 0x08, 0x74, 0x20, 0x08,
    0x00, 0x05, 0x70, 0x03, 0x00, 0xa8, 0x1e, 0x2b, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x2c, 0x0c, 0x2c,
    0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x0c, 0x2c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0xb5, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc2, 0x27, 0x3d,
    0x11, 0x11, 0x04, 0x05, 0x00, 0x86, 0x32, 0xb1, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0x45,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x38, 0x41, 0x53, 0x46, 0x31, 0x47,
    0x37, 0x32, 0x50, 0x44, 0x5a, 0x2d, 0x32, 0x47, 0x33, 0x41, 0x32, 0x20, 0x32, 0x80, 0x2c, 0x41,
    0x41, 0x42, 0x5a, 0x41, 0x48, 0x4c, 0x35, 0x35, 0x30, 0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// SPD_MODULE_TYPE_DDR4_RDIMM : DDR4-2400 Samsung
uint8_t spd_ddr4_2Rx4_16GB_rdimm[] =       //!< M393A2G40DB1-CRC 2400 : spd_ddr4_2Rx4_16GB_SAM.bin
{
    0x23, 0x11, 0x0c, 0x01, 0x84, 0x21, 0x00, 0x08, 0x00, 0x40, 0x00, 0x03, 0x08, 0x0b, 0x80, 0x00,
    0x00, 0x00, 0x07, 0x0d, 0xf8, 0x0f, 0x00, 0x00, 0x6e, 0x6e, 0x6e, 0x11, 0x00, 0x6e, 0x20, 0x08,
    0x00, 0x05, 0x70, 0x03, 0x00, 0x68, 0x1b, 0x28, 0x28, 0x00, 0x78, 0x00, 0x14, 0x3c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x16, 0x16, 0x16,
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0xb5, 0x00, 0x00, 0x00, 0x00, 0xe7, 0xd6, 0x92, 0x2a,
    0x11, 0x11, 0x20, 0x09, 0x00, 0x80, 0xb3, 0x30, 0x01, 0x65, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x15,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0xce, 0x01, 0x15, 0x20, 0x02, 0xc2, 0xb2, 0x08, 0x4d, 0x33, 0x39, 0x33, 0x41, 0x32, 0x47,
    0x34, 0x30, 0x44, 0x42, 0x31, 0x2d, 0x43, 0x52, 0x43, 0x20, 0x20, 0x20, 0x20, 0x00, 0x80, 0xce,
    0x00, 0x33, 0x30, 0x4f, 0x44, 0x30, 0x30, 0x30, 0x23, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// SPD_MODULE_TYPE_DDR4_RDIMM : RDIMM 16GB 1Rx4 2667  Micron
uint8_t spd_ddr4_1Rx4_16GB_rdimm[] =       //!< MTA18ASF2G72PZ-2G6D1 : bringup1a.bin
{
    0x23, 0x11, 0x0c, 0x01, 0x85, 0x29, 0x00, 0x08, 0x00, 0x60, 0x00, 0x03, 0x00, 0x0b, 0x80, 0x00,
    0x00, 0x00, 0x06, 0x0d, 0xf8, 0x3f, 0x00, 0x00, 0x6e, 0x6e, 0x6e, 0x11, 0x00, 0x6e, 0xf0, 0x0a,
    0x20, 0x08, 0x00, 0x05, 0x00, 0x60, 0x18, 0x28, 0x28, 0x00, 0x78, 0x00, 0x14, 0x3c, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x0d, 0x15, 0x0d,
    0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x15, 0x0d, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe7, 0x00, 0xbe, 0x52,
    0x11, 0x11, 0x22, 0x15, 0x00, 0x80, 0xb3, 0x50, 0x00, 0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xeb, 0x97,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x2c, 0x0f, 0x16, 0x22, 0x12, 0xde, 0xfd, 0xb8, 0x31, 0x38, 0x41, 0x53, 0x46, 0x32, 0x47,
    0x37, 0x32, 0x50, 0x5a, 0x2d, 0x32, 0x47, 0x36, 0x44, 0x31, 0x20, 0x20, 0x20, 0x31, 0x80, 0x2c,
    0x44, 0x44, 0x50, 0x41, 0x47, 0x47, 0x31, 0x35, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* @} */


/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Function Prototypes
******************************************************************************/
void spd_simm_get_sim_data(target_mode_e mode);


/******************************************************************************
* External Functions
******************************************************************************/


/*!****************************************************************************
*
* @par Description:
*    This function initialization the SPD simulation mechanism.
*
*                                  @param
*    mode           The mode of the target being executed on.
*                     [SILICON|RUMI42|RUMI48|SIM|SIM_RUMI42|SIM_RUMI48]
*
*
* @return
*    None
*
*
******************************************************************************/
void spd_simm_init
(
    target_mode_e mode
)
{
    // Flag use of simulated spd data
    spd_use_simulated_spd = true;


    // Override the SMBus EEPROM addresses so that they are invalid if the chan/slot
    //  is invalid for the given simulation mode.This allows the simulation code to
    //  identify invalid channels later. The used addresses are also written so that
    //  the init can be run multiple times for test purposes and the unused
    //  channels/slots in one scenario can be used in another.

    mddr_log(INFORMATIONAL, "Loading simulated SPD data\n");

    if (mode == CHIPSIM)
    {
        // Determine the phy type and select the corresponding DDR4 SPD data array
        spd_simm_get_sim_data(mode);

        spd_smbus_info[0][1].spd_eeprom_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][0].spd_eeprom_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][1].spd_eeprom_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[0][1].spd_ts_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][0].spd_ts_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][1].spd_ts_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[0][1].spd_cmd_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][0].spd_cmd_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][1].spd_cmd_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;

    }
    else if (mode == SILICON)
    {
        // Record the selected simulated SPD source.
        spd_sim_array_ptr = spd_ddr4_1Rx4_16GB_rdimm; //!< MTA18ASF2G72PZ-2G6D1 : bringup1a.bin

    }
    else if (mode == PALLADIUM_Z1)
    {
        // --TBD--
        // Record the selected simulated SPD source.
        // Generic ddr3 spd (DDR3 UDIMM 2Rx8 1600 8GB BRC=3x16x10)
        spd_sim_array_ptr = spd_ddr4_1Rx4_8GB_rdimm;


        // Four channels both slots are valid for sim to allow for granule testing.
        //  So mark the other two channels as unpopulated.
        spd_smbus_info[0][1].spd_eeprom_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][1].spd_eeprom_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[0][1].spd_ts_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][1].spd_ts_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[0][1].spd_cmd_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;
        spd_smbus_info[1][1].spd_cmd_bus_addr = SPD_SIMM_UNPOPULATED_MARKER;

    }
}



/*!****************************************************************************
*
* @par Description:
*    This function gets a block of SPD serial EEPROM information from simulation.
*
*                                  @param
*    smbus_id           The SMBus ID where the SPD information is located.
*                                  @param
*    spd_bus_addr       The SMBus address of the SPD information to be retrieved.
*                                  @param
*    spd_reg_id         The SPD register id of the start of the information to
*                        be retrieved.
*                                  @param
*    xfer_block_size    The size of the block of SPD information to retrieve.
*                                  @param
*    dst_buff_addr      The address of the buffer to receive the SPD information.
*
*
* @return
*    COM_OK if the slot is considered populated, COM_ERROR otherwise.
*
*
* @par Notes:
*    In simulation mode, all SMBus addresses that are associated to unpopulated
*    slots have their bus address set to a special marker value to indicate the
*    fact.
*
******************************************************************************/
unsigned spd_simm_get_spd_block
(
    UNUSED_PARAM uint8_t smbus_id,
    uint8_t spd_bus_addr,
    uint16_t spd_reg_id,
    uint32_t xfer_block_size,
    uint64_t dst_buff_addr
)
{
    unsigned status = COM_OK;
    unsigned byte_num;

    if (spd_bus_addr == SPD_SIMM_UNPOPULATED_MARKER)
    {
        status = COM_ERROR;
    }
    else
    {
        for(byte_num = 0; byte_num < xfer_block_size; ++byte_num)
        {
            // The actual target block starts with the indicated register ID
            write8(dst_buff_addr + byte_num, spd_sim_array_ptr[spd_reg_id + byte_num]);
        }
    }

    return status;
}



/*!****************************************************************************
*
* @par Description:
*    This function simulates getting a word of SPD Thermal Sensor information.
*
*                                  @param
*    smbus_id         The SMBus ID where the SPD information is located.
*                                  @param
*    spd_bus_addr     The SMBus address of the SPD information to be retrieved.
*                                  @param
*    spd_reg_id       The SPD register id of the start of the information to
*                      be retrieved.
*                                  @param
*    spd_data_ptr     Pointer to where the SPD data word is to be stored. The
*                      target location is not updated if the simulated bus
*                      address is unpopulated.
*
* @return
*    Status indicating if the data could be retrieved from the indicated SMBus.
*
* @par Notes:
*    In simulation mode, all SMBus addresses that are associated to unpopulated
*     slots have their bus address set to a special marker value to indicate the
*     fact.
*    This function is used in DIMM temp testing and so data is generated by
*     this function to provide what looks like signed temperature data that
*     varies by target bus address.
*
******************************************************************************/
unsigned spd_simm_get_word
(
    uint8_t smbus_id,
    uint8_t spd_bus_addr,
    UNUSED_PARAM uint16_t spd_reg_id,
    uint16_t *spd_data_ptr
)
{
    unsigned status = COM_ERROR;

    if (spd_bus_addr != SPD_SIMM_UNPOPULATED_MARKER)
    {
        status = COM_OK;

        // Use the address as dummy data to track the target device.
        if((smbus_id % 2) == 0)
        {
            // Give even IDs positive values
            *spd_data_ptr = spd_bus_addr;
        }
        else
        {
            // Give odd IDs negative values
            *spd_data_ptr = ~spd_bus_addr + 1;
        }
    }

    return status;
}



/*!****************************************************************************
*
* @par Description:
*    This function gets a byte of simulated SPD serial EEPROM information.
*
*                                  @param
*    smbus_id         The SMBus ID where the SPD information is located.
*                                  @param
*    spd_bus_addr     The SMBus address of the SPD information to be retrieved.
*                                  @param
*    spd_reg_id       The SPD register id of the start of the information to
*                      be retrieved.
*                                  @param
*    spd_data_ptr     Pointer to where the SPD data word is to be stored. The
*                      target location is not updated if the simulated bus
*                      address is unpopulated.
*
* @return
*    Status indicating if the data could be retrieved from the indicated SMBus.
*
*
* @par Notes:
*    In simulation mode, all SMBus addresses that are associated to unpopulated
*    slots have their bus address set to a special marker value to indicate the
*    fact.
*
******************************************************************************/
unsigned spd_simm_get_byte
(
    UNUSED_PARAM uint8_t smbus_id,
    uint8_t spd_bus_addr,
    uint16_t spd_reg_id,
    uint8_t *spd_data_ptr
)
{
    unsigned status = COM_ERROR;

    if (spd_bus_addr != SPD_SIMM_UNPOPULATED_MARKER)
    {
        status = COM_OK;

        // Get the data directly from the simulated SPD EEPROM data
        *spd_data_ptr = spd_sim_array_ptr[spd_reg_id];
    }

    return status;
}



/*!****************************************************************************
*
* @par Description:
*    This function determines the type of DDR and selects the corresponding
*     DDR3/DDR4 SPD data array.
*
*                                  @param
*    mode           The mode of the target being executed on.
*                     [SILICON|RUMI42|RUMI48|SIM|SIM_RUMI42|SIM_RUMI48]
*
* @return
*    None.
*
*
******************************************************************************/
void spd_simm_get_sim_data(target_mode_e mode)
{
    uint32_t ddr_type;

    // Since DDR channel 0 slot 0 is populated in both RUMI42 and RUMI48,
    // using this channel to determine the PHY type.
    uint8_t ddr_chan = 0x0;

    // Determine DDR type based on which DDR3 or DDR4 is determined
    ddr_type = mddr_determine_ddr_type(mode, ddr_chan);

    //MDDR_TEST_STATUS[27:24] = 0x4 implies DDR4 else DDR3
    if (ddr_type==0x4)    // DDR4
    {
        // Record the selected simulated SPD source - DDR4
        spd_sim_array_ptr = spd_ddr4_2Rx4_16GB_rdimm;
    }
}


/*!****************************************************************************
*
* @par Description:
*    This function simulates setting a word of SPD Thermal Sensor information.
*
*                                  @param
*    smbus_id         The SMBus ID where the SPD information is located.
*                                  @param
*    spd_bus_addr     The SMBus address of the SPD information to be set.
*                                  @param
*    spd_reg_id       The SPD register id of the start of the information to
*                      be set.
*                                  @param
*    spd_data         The SPD data to be written. The target location is not
*                      updated if the simulated bus address is unpopulated.
*
* @return
*    Status indicating if the data could be set to the indicated SMBus.
*
*
* @par Notes:
*    In simulation mode, all SMBus addresses that are associated to unpopulated
*     slots have their bus address set to a special marker value to indicate the
*     fact.
*    This function is used in DIMM temp testing and so data is generated by
*     this function to provide what looks like signed temperature data that
*     varies by target bus address.
*
******************************************************************************/
unsigned spd_simm_set_word
(
    UNUSED_PARAM uint8_t smbus_id,
    uint8_t spd_bus_addr,
    uint16_t spd_reg_id,
    uint16_t spd_data
)
{
    unsigned status = COM_ERROR;

    if (spd_bus_addr != SPD_SIMM_UNPOPULATED_MARKER)
    {
        status = COM_OK;

        spd_sim_array_ptr[spd_reg_id] = (spd_data >> BITS_PER_BYTE) & 0xFF;

        spd_sim_array_ptr[spd_reg_id + 1] = spd_data & 0xFF;;
    }

    return status;
}


/*!****************************************************************************
*
* @par Description:
*    This function sets a byte of simulated SPD serial EEPROM information.
*
*                                  @param
*    smbus_id         The SMBus ID where the SPD information is located.
*                                  @param
*    spd_bus_addr     The SMBus address of the SPD information to be set.
*                                  @param
*    spd_reg_id       The SPD register id of the start of the information to
*                      be set.
*                                  @param
*    spd_data         The SPD data to be written. The target location is not
*                      updated if the simulated bus address is unpopulated.
*
* @return
*    Status indicating if the data could be set to the indicated SMBus.
*
*
* @par Notes:
*    In simulation mode, all SMBus addresses that are associated to unpopulated
*    slots have their bus address set to a special marker value to indicate the
*    fact.
*
******************************************************************************/
unsigned spd_simm_set_byte
(
    UNUSED_PARAM uint8_t smbus_id,
    uint8_t spd_bus_addr,
    uint16_t spd_reg_id,
    uint8_t spd_data
)
{
    unsigned status = COM_ERROR;

    if (spd_bus_addr != SPD_SIMM_UNPOPULATED_MARKER)
    {
        status = COM_OK;

        // Set the data directly to the simulated SPD EEPROM data
        spd_sim_array_ptr[spd_reg_id] = spd_data;
    }

    return status;
}

/******************************************************************************
* Local Functions
******************************************************************************/
