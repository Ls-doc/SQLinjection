#ifndef LYTRAX_TEST_H
#define LYTRAX_TEST_H

extern "C" {
#include "php.h"
}

// A very simple test class
class Test {
public:
    Test(int maxGear);
    void shift(int gear);
    void accelerate();
    void brake();
    int getCurrentSpeed();
    int getCurrentGear();
public:
    int maxGear;
    int currentGear;
    int speed;
public:
    // Here we store our callback function
    zend_fcall_info fci_onTest;
    zend_fcall_info_cache fcc_onTest;
};

#endif /* LYTRAX_TEST_H */
