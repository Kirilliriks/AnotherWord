//
// Created by KirilliriK on 12.03.2021.
//

#include "Screen.h"

Screen::Screen(const int nWidth, const int nHeight) {
    width = (short) nWidth;
    height = (short) nHeight;
    dwordBytes = 0;
    maxSize = Vector(width, height);
    cursorPos = Vector(15, 0);
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    if (console == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Invalid handle output");
    if (inputHandle == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Invalid handle input");

    SMALL_RECT rectWindow = { 0, 0, (short)width, (short)height};
    SetConsoleScreenBufferSize(console, {(SHORT)maxSize.x, (SHORT)maxSize.y});
    SetConsoleWindowInfo(console, true, &rectWindow);
    SetConsoleTitleA("AnotherWord");

    DWORD fwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE;
    if (!SetConsoleMode(inputHandle, fwMode))
        throw std::runtime_error("ConsoleMode error");
}

void Screen::draw(wchar_t *charBuffer, WORD *colorBuffer) {
    if (charBuffer == nullptr || colorBuffer == nullptr)
        throw std::runtime_error("Char or color buffer null");
    WriteConsoleOutputCharacterW(console, charBuffer, width * height, {0, 0}, &dwordBytes);
    WriteConsoleOutputAttribute(console, colorBuffer, width * height, {0, 0}, &dwordBytes);
}

void Screen::setCursor(Vector vector) {
    Vector newPos = vector;
    if (newPos.x < 0 || newPos.x > width || newPos.y < 0 || newPos.y > height) return;
    cursorPos = vector;
    SetConsoleCursorPosition(console, cursorPos);
}

Vector Screen::getMaxSize() {
    return maxSize;
}

Vector Screen::getCursorPosition() {
    return cursorPos;
}

Screen::~Screen() = default;
