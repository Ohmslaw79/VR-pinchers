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
    return (String)"Charge Terminated: " + (String)((bool) (status & 0x1)) + \
        (String)"\n Active Empty:      " + (String)((bool) (status & 0x2)) + \
        (String)"\n Standby Empty:     " + (String)((bool) (status & 0x3)) + \
        (String)"\n Learning:          " + (String)((bool) (status & 0x4)) + \
        (String)"\n UnderVoltage:      " + (String)((bool) (status & 0x6));
}

bool DS2780::set_rsns(uint8_t rsns_ohms)
{
    //Final Units 1/rsns ohms
    init();
    rsnsp = 1 / rsns_ohms;
    buf[0] = CMD_WRITE;
    buf[1] = EEPROM_RSNSP;
    buf[2] = rsnsp;
    bus.write_bytes(buf, 3);

    buf[0] = CMD_READ;
    bus.write_bytes(buf, 2);

    if (bus.read() == rsnsp)
    {
        buf[0] = CMD_COPY;
        buf[1] = EEPROM_RSNSP;
        bus.write_bytes(buf, 2);
        return true;
    }
    return false;
}

void DS2780::set_charge_voltage(float voltage)
{
    //Final Units 19.52mV (1 byte)

    return false;
}

void DS2780::set_active_empty_voltage(float voltage)
{
    //Final Units 19.52mV (1 byte)

    return false;
}

void DS2780::set_active_empty_current(float current)
{
    //Units 200 uV (IAE * rsns)
}

void DS2780::set_min_charge_current(float min_current)
{
    //Final units 50uV (IMIN * RSNS) (1 byte)

    return false;
}

void DS2780::set_aging_capacity(int battery_capcity_mah)
{
    //Final units 6.25uVh (2 bytes)
}

bool DS2780::set_eeprom_parameters()
{
    return false;
}

bool DS2780::check_eeprom_parameters()
{
    return false;
}
