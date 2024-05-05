#pragma once

#include <exception>
#include <fstream>
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

class UeventKey {
public:
    enum Kind {
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
    };

    UeventKey() : value(Kind::INVALID){};
    explicit UeventKey(const std::string& s);
    // bool operator==(const UeventKey& other) const;

    Kind get() const;

protected:
    Kind value;
};

} // namespace libBB

/*
 * to get the dumb hashing working
 */
namespace std {
template <> struct hash<libBB::UeventKey> {
    size_t operator()(const libBB::UeventKey& key) const {
        // Simple example: hash based on the underlying enum value
        return std::hash<int>()(static_cast<int>(key.get()));
    }
};
} // namespace std

namespace libBB {
class Uevent {
public:
    Uevent(const Config& cfg);
    ~Uevent();

    void refresh();
    auto operator[](const UeventKey& key) -> const std::string&;

protected:
    std::unordered_map<UeventKey, std::string> state;
    std::ifstream file;
};

class Capacity {
public:
    Capacity();
    ~Capacity();
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
