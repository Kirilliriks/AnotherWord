//
// Created by KirilliriK on 22.03.2021.
//

#ifndef ANOTHERWORD_TIMER_H
#define ANOTHERWORD_TIMER_H

#include <chrono>

class Timer {
public:
    Timer();
    float getDelta();
private:
    std::chrono::time_point<std::chrono::steady_clock> delta;
    std::chrono::duration<float> duration;
};


#endif //ANOTHERWORD_TIMER_H
