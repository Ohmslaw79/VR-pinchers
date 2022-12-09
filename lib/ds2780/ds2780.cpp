#include "ds2780.h"

uint8_t DS2780::get_status()
{
    init();
    buf[0] = CMD_READ;
    buf[1] = STATUS;
    bus.write_bytes(buf, 2);
    return bus.read();
}

void DS2780::init()
{
    bus.reset();
    bus.skip();
}

bool DS2780::check_eeprom_value(uint8_t address, uint8_t value)
{
    // Load value from EEPROM to memory
    init();
    buf[0] = CMD_RECALL;
    buf[1] = address;
    bus.write_bytes(buf, 2);
    // Read values from memory
    init();
    buf[0] = CMD_READ;
    bus.write_bytes(buf, 2);
    bus.read_bytes(&buf[2], 2);

    return value == buf[2];
}

bool DS2780::write_eeprom_value(uint8_t address, uint8_t value)
{
    if (!check_eeprom_value(address, value))
    {
        init();
        buf[0] = CMD_WRITE;
        buf[1] = address;
        buf[2] = value;
        bus.write_bytes(buf, 3);

        init();
        buf[0] = CMD_COPY;
        bus.write_bytes(buf, 2);
    }
    return check_eeprom_value(address, value);
}

DS2780::DS2780(int pin, int _battery_capacity) : bus(pin)
{
    battery_capacity = _battery_capacity;
}

int DS2780::get_battery_percentage()
{
    init();
    buf[0] = CMD_READ;
    buf[1] = RAAC_MSB;
    bus.write_bytes(buf, 2);
    bus.read_bytes(buf, 2);
    uint16_t mah_remaining = mah_remaining;

    return mah_remaining / BATT_CAPACITY_MAH;
}

float DS2780::get_battery_voltage()
{
    init();
    buf[0] = CMD_READ;
    buf[1] = VOLT_MSB;
    bus.write_bytes(buf, 2);
    bus.read_bytes(buf, 2);
    uint16_t data = (buf[0] << 3) | (buf[1] >> 5);
    float voltage = data * .00488;

    return voltage;
}

float DS2780::get_battery_current()
{
    init();
    buf[0] = CMD_READ;
    buf[1] = CURRENT_MSB;
    bus.write_bytes(buf, 2);
    bus.read_bytes(buf, 2);
    uint16_t data = (buf[0] << 8) | buf[1];
    return data / 0.0015625;
}

uint64_t DS2780::get_net_address()
{
    bus.reset();
    bus.write(NET_READ);
    bus.read_bytes(buf, 8);
    uint64_t output = 0;
    for (int i = 0; i < 8; i++)
    {
        output <<= 8;
        output |= buf[i];
    }
    return output;
}

String DS2780::get_formatted_status()
{
    uint8_t status = get_status();
    return (String) "Charge Terminated: " + (String)((bool)(status & 0x01)) +
           (String) "\n Active Empty:      " + (String)((bool)(status & 0x02)) +
           (String) "\n Standby Empty:     " + (String)((bool)(status & 0x04)) +
           (String) "\n Learning:          " + (String)((bool)(status & 0x08)) +
           (String) "\n UnderVoltage:      " + (String)((bool)(status & 0x20));
}

bool DS2780::set_rsns(float rsns_ohms)
{
    // Final Units 1/rsns ohms
    return write_eeprom_value(EEPROM_RSNSP, 1 / rsns_ohms);
}

bool DS2780::set_charge_voltage(float voltage_v)
{
    // Final Units 19.52mV (1 byte)
    return write_eeprom_value(EEPROM_VCHG, voltage_v * (1000 / 19.52));
}

bool DS2780::set_active_empty_voltage(float voltage_v)
{
    // Final Units 19.52mV (1 byte)
    return write_eeprom_value(EEPROM_VAE, voltage_v * (1000 / 19.52));
}

bool DS2780::set_active_empty_current(float current_mA)
{
    // Units 200 uV (IAE * rsns)
    return write_eeprom_value(EEPROM_IAE, current_mA * (1000 / 200));
}

bool DS2780::set_min_charge_current(float min_current_mA)
{
    // Final units 50uV (IMIN * RSNS) (1 byte)
    return write_eeprom_value(EEPROM_IMIN, min_current_mA * (1000 / 50));
}

bool DS2780::set_aging_capacity(int battery_capacity_mah, float rsns_ohms)
{
    // Final units 6.25uVh (2 bytes)
    uint16_t aging_capacity = battery_capacity_mah * rsns_ohms * 1000;
    return write_eeprom_value(EEPROM_AC_LSB, (uint8_t)aging_capacity) &&
           write_eeprom_value(EEPROM_AC_LSB, (uint8_t)(aging_capacity >> 8));
}
