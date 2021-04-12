//
// Created by KirilliriK on 22.03.2021.
//

#include "Util.h"

bool Util::isKeyPress(int key) {
    return GetKeyState(key) & 0x8000;
}
