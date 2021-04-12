//
// Created by KirilliriK on 22.03.2021.
//

#include "Timer.h"

Timer::Timer() {
    delta = std::chrono::high_resolution_clock::now();
}

float Timer::getDelta() {
    duration = std::chrono::high_resolution_clock::now() - delta;
    delta = std::chrono::high_resolution_clock::now();
    return duration.count() * 1000.0f;
}
