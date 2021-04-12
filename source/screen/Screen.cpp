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
    if (console == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Invalid handle");
    SMALL_RECT rectWindow = { 0, 0, (short)width, (short)height};
    DWORD fwMode = ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
    SetConsoleMode(console, fwMode);
    SetConsoleScreenBufferSize(console, {(SHORT)maxSize.x, (SHORT)maxSize.y});
    SetConsoleWindowInfo(console, true, &rectWindow);
    SetConsoleTitleA("AnotherWord");
}

void Screen::draw(wchar_t *charBuffer, WORD *colorBuffer) {
    if (charBuffer == nullptr || colorBuffer == nullptr)
        throw std::runtime_error("Char or color buffer null");
    WriteConsoleOutputCharacterW(console, charBuffer, width * height, {0, 0}, &dwordBytes);
    WriteConsoleOutputAttribute(console, colorBuffer, width * height, {0, 0}, &dwordBytes);
}

void Screen::moveCursor(Vector vector) {
    Vector newPos = cursorPos + vector;
    if (newPos.x < 0 || newPos.x > width || newPos.y < 0 || newPos.y > height) return;
    cursorPos += vector;
    SetConsoleCursorPosition(console, cursorPos);
}

Vector Screen::getMaxSize() {
    return maxSize;
}

Vector Screen::getCursorPos() {
    return cursorPos;
}

HANDLE *Screen::getConsole() {
    return &console;
}

Screen::~Screen() {

}
