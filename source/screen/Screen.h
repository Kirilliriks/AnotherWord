//
// Created by KirilliriK on 12.03.2021.
//

#ifndef ANOTHERWORD_SCREEN_H
#define ANOTHERWORD_SCREEN_H

#include <iostream>
#include <fstream>
#include <windows.h>

#include "char/Color.h"
#include "vector/Vector.h"

class Screen {
public:
    Screen(int nWidth, int nHeight);
    ~Screen();
    void draw(wchar_t *charBuffer, WORD *colorBuffer); // Отрисовывает экран консоль;
    void setCursor(Vector vector);
    Vector getMaxSize();
    Vector getCursorPosition();
private:
    short width, height;
    Vector maxSize;
    Vector cursorPos;
    HANDLE console;
    HANDLE inputHandle;
    DWORD dwordBytes;
};


#endif //ANOTHERWORD_SCREEN_H
