#include "libBB.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

namespace libBB {

/**
 * @param s This is a string with "POWER_SUPPLY_" chopped off.
 */
UeventKey::UeventKey(const std::string& s) {

    using K = UeventKey::Kind;
    std::unordered_map<std::string, K> map = {
        {"NAME",               K::NAME              },
        {"TYPE",               K::TYPE              },
        {"STATUS",             K::STATUS            },
        {"PRESENT",            K::PRESENT           },
        {"TECHNOLOGY",         K::TECHNOLOGY        },
        {"CYCLE_COUNT",        K::CYCLE_COUNT       },
        {"VOLTAGE_MIN_DESIGN", K::VOLTAGE_MIN_DESIGN},
        {"VOLTAGE_NOW",        K::VOLTAGE_NOW       },
        {"POWER_NOW",          K::POWER_NOW         },
        {"ENERGY_FULL_DESIGN", K::ENERGY_FULL_DESIGN},
        {"ENERGY_FULL",        K::ENERGY_FULL       },
        {"ENERGY_NOW",         K::ENERGY_NOW        },
        {"CAPACITY",           K::CAPACITY          },
        {"CAPACITY_LEVEL",     K::CAPACITY_LEVEL    },
        {"MODEL_NAME",         K::MODEL_NAME        },
        {"MANUFACTURER",       K::MANUFACTURER      },
        {"SERIAL_NUMBER",      K::SERIAL_NUMBER     },
    };

    this->value = map[s];
}

/**
 * @throws libBB::InvalidBatteryException
 */
Uevent::Uevent(const Config& cfg) {
    fs::path fpath = fs::path("/sys/class/power_supply") / cfg.bat;

    if (!fs::exists(fpath)) {
        std::cerr << "invalid battery\n";
        throw InvalidBatteryException(cfg.bat);
    }

    fpath /= "uevent";

    this->file = std::ifstream(fpath);
    if (!this->file) {
        std::cerr << "failed to open file\n";
        std::exit(1);
    }

    this->refresh();
}

void Uevent::refresh() {
    std::string line;
    std::string key;
    std::string value;

    while (std::getline(this->file, line)) {
        auto equalsPos = line.find('=');
        key = line.substr(13, equalsPos);
        value = line.substr(equalsPos);

        this->state[UeventKey(key)] = value;
    }
}

auto Uevent::operator[](const UeventKey& key) -> const std::string& {
    return this->state[key];
}

} // namespace libBB
