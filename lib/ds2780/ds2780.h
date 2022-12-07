#ifndef DS2780_H_
#define DS2780_H_

#include <OneWire.h>
#include <String.h>

#define NET_READ 0x33
#define NET_MATCH 0x55
#define NET_SKIP 0xCC
#define NET_SEARCH 0xF0
#define NET_RESUME 0xA5

#define CMD_READ 0x69
#define CMD_WRITE 0x6C
#define CMD_COPY 0x48
#define CMD_RECALL 0xB8
#define CMD_LOCK 0x6A

#define STATUS 0x01
#define RAAC_MSB 0x02
#define RAAC_LSB 0x03
#define RSAC_MSB 0x04
#define RSAC_LSB 0x05
#define RARC 0x06
#define RSRC 0x07
#define IAVG_MSB 0x08
#define IAVG_LSB 0x09
#define TEMP_MSB 0x0A
#define TEMP_LSB 0x0B
#define VOLT_MSB 0x0C
#define VOLT_LSB 0x0D
#define CURRENT_MSB 0x0E
#define CURRENT_LSB 0x0F
#define ACR_MSB 0x10
#define ACR_LSB 0x11
#define ACRL_MSB 0x12
#define ACRL_LSB 0x13
#define AS 0x14
#define SFR 0x15
#define FULL_MSB 0x16
#define FULL_LSB 0x17
#define AE_MSB 0x18
#define AE_LSB 0x19
#define SE_MSB 0x1A
#define SE_LSB 0x1B
#define EEPROM 0x1F
#define EEPROM_USER 0x20
#define EEPROM_CONTROL 0x60
#define EEPROM_AB 0x61
#define EEPROM_AC_MSB 0x62
#define EEPROM_AC_LSB 0x63
#define EEPROM_VCHG 0x64
#define EEPROM_IMIN 0x65
#define EEPROM_VAE 0x66
#define EEPROM_IAE 0x67
#define EEPROM_RSNSP 0x69
#define EEPROM_RSGAIN_MSB 0x78
#define EEPROM_RSGAIN_LSB 0x79
#define EEPROM_RSTC 0x7A
#define EEPROM_FRSGAIN_MSB 0x7B
#define EEPROM_FRSGAIN_LSB 0x7C

#define BATT_CAPACITY_MAH 2000

class DS2780
{
private:
    OneWire bus;
    uint8_t* buf = new uint8_t(8);
    uint8_t get_status();
    void init();

public:
    DS2780(int pin);
    int get_battery_percentage();
    float get_battery_voltage();
    float get_battery_current();
    uint64_t get_net_address();
    String get_formatted_status();
    bool set_rsns(uint8_t rsns_ohms);
};

#endif /* DS2780_H_ */
