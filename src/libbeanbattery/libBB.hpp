#pragma once

#include <exception>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

namespace libBB {

struct Config {
    std::string bat;
};

class InvalidBatteryException : std::exception {
protected:
    std::string bat;
    std::string fullMsg;

public:
    InvalidBatteryException(const std::string& bat) : bat(bat) {
        this->fullMsg = "The battery '" + bat + "' does not exist.";
    }

    const char* what() const noexcept override { return this->fullMsg.c_str(); }
};

enum UeventKey {
    INVALID = 0,
    NAME,
    TYPE,
    STATUS,
    PRESENT,
    TECHNOLOGY,
    CYCLE_COUNT,
    VOLTAGE_MIN_DESIGN,
    VOLTAGE_NOW,
    POWER_NOW,
    ENERGY_FULL_DESIGN,
    ENERGY_FULL,
    ENERGY_NOW,
    CAPACITY,
    CAPACITY_LEVEL,
    MODEL_NAME,
    MANUFACTURER,
    SERIAL_NUMBER,
}; // namespace libBB

class Uevent {
public:
    Uevent(const Config& cfg);
    ~Uevent();

    void refresh();
    auto get(const UeventKey& key) const -> const std::string&;

protected:
    std::unordered_map<UeventKey, std::string> state;
    std::ifstream file;
};

class Capacity {
public:
    enum Level {
        EMPTY = 0,
        CRITICAL,
        LOW,
        NORMAL,
        HIGH,
        FULL,
    };

    Capacity();
    Capacity(const Uevent& uev);
    ~Capacity();
    void refresh();

protected:
    int cycleCount;
    int capacity;
    Level chargeLevel;

    void setChargeLevel();
};

class ChargeInfo {
public:
    ChargeInfo();
    ~ChargeInfo();
};

class BatInfo {
public:
    BatInfo();
    ~BatInfo();
};

class Battery {
public:
    Battery();
    ~Battery();
};

} // namespace libBB
