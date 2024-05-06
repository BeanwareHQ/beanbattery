#include "libBB.hpp"

namespace libBB {

Capacity::Capacity(const Uevent& uev, Config& cfg) : cfg(cfg) {
    this->refresh(uev);
}

void Capacity::refresh(const Uevent& uev) {
    this->cycleCount = std::stoi(uev.get(UeventKey::CYCLE_COUNT));
    this->capacity = std::stoi(uev.get(UeventKey::CAPACITY));
    this->setChargeLevel();
}

void Capacity::setChargeLevel() {
    using C = Capacity::Level;
    int& v = this->capacity;

    if (v == 0)
        this->chargeLevel = C::EMPTY;
    else if (v > 0 && v < this->cfg.criticalThreshold)
        this->chargeLevel = C::CRITICAL;
    else if (v > this->cfg.criticalThreshold && v < this->cfg.lowThreshold)
        this->chargeLevel = C::LOW;
    else if (v > this->cfg.lowThreshold && v < this->cfg.fullAt)
        this->chargeLevel = C::NORMAL;
    else
        this->chargeLevel = C::FULL;
}

} // namespace libBB
