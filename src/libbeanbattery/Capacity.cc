#include "libBB.hpp"
namespace libBB {

Capacity::Capacity(const Uevent& uev) {
    this->cycleCount = std::stoi(uev.get(UeventKey::CYCLE_COUNT));
    this->capacity = std::stoi(uev.get(UeventKey::CAPACITY));
    this->setChargeLevel();
}

void Capacity::refresh() {}

} // namespace libBB
