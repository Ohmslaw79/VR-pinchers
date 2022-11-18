#ifndef DS2780_H_
#define DS2780_H_

#include  <owb.h>
#include <owb_gpio.h>
#include <owb_rmt.h>

#define STATUS 01
#define RAAC_MSB 02
#define RAAC_LSB 03
#define RSAC_MSB 04
#define RSAC_LSB 05
#define RARC 06
#define RSRC 07
#define IAVG_MSB 08
#define IAVG_LSB 09
#define TEMP_MSB 0A
#define TEMP_LSB 0B
#define VOLT_MSB 0C
#define VOLT_LSB 0D
#define CURRENT_MSB 0E
#define CURRENT_LSB 0F
#define ACR_MSB 10
#define ACR_LSB 11
#define ACRL_MSB 12
#define ACRL_LSB 13
#define AS 14
#define SFR 15
#define FULL_MSB 16
#define FULL_LSB 17
#define AE_MSB 18
#define AE_LSB 19
#define SE_MSB 1A
#define SE_LSB 1B
#define EEPROM 1F
#define EEPROM_USER 20
#define EEPROM_CONTROL 60
#define EEPROM_AB 61
#define EEPROM_AC_MSB 62
#define EEPROM_AC_LSB 63
#define EEPROM_VCHG 64
#define EEPROM_IMIN 65
#define EEPROM_VAE 66
#define EEPROM_IAE 67
#define EEPROM_RSNSP 69
#define EEPROM_RSGAIN_MSB 78
#define EEPROM_RSGAIN_LSB 79
#define EEPROM_RSTC 7A
#define EEPROM_FRSGAIN_MSB 7B
#define EEPROM_FRSGAIN_LSB 7C

#endif /* DS2780_H_ */
