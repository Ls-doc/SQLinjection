#include "test.h"

Test::Test(int maxGear) {
    this->maxGear = maxGear;
    this->currentGear = 1;
    this->speed = 0;
}

void Test::shift(int gear) {
    if (gear < 1 || gear > maxGear) {
        return;
    }
    currentGear = gear;
}

void Test::accelerate() {
    speed += (5 * this->getCurrentGear());
}

void Test::brake() {
    speed -= (5 * this->getCurrentGear());
}

int Test::getCurrentSpeed() {
    return speed;
}

int Test::getCurrentGear() {
    return currentGear;
}
