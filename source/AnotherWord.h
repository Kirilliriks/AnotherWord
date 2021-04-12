//
// Created by KirilliriK on 12.03.2021.
//

#ifndef ANOTHERWORD_ANOTHERWORD_H
#define ANOTHERWORD_ANOTHERWORD_H

#include <vector>
#include <string>
#include <istream>
#include <chrono>
#include <windows.h>

#include "screen/Screen.h"
#include "util/Util.h"
#include "util/timer/Timer.h"
#include "conio.h"

class AnotherWord {
public:
    AnotherWord();
    ~AnotherWord();
    void start();
    void update(float deltaTime);
    void draw(float deltaTime);
    void loadFile(const std::string& path);
    void clearBuffers();
private:
    void handleInput(float deltaTime);
    void handleKeyboard(float deltaTime);
    // Методы для взаимодействия с текстом
    void writeChar(char ch);
    void clearChar();
    void putChar(char ch, Vector vector);
    // Методы для отрисовки интерфейса программы
    void drawChar(char ch, int x, int y, Color color = Color::White, BackgroundColor backColor = BackgroundColor::Black);
    void drawChar(char ch, Vector vector, Color color = Color::White, BackgroundColor backColor = BackgroundColor::Black);
    void drawLine(char ch, int x1, int y1, int x2, int y2, Color color = Color::White, BackgroundColor backColor = BackgroundColor::Black);
    void drawString(std::string str, int x, int y, Color color = Color::White, BackgroundColor backColor = BackgroundColor::Black);
    // Логический обработчик перемешения мыши
    void moveCursor(Vector moveVector);
private:
    Vector screenSize;
    HANDLE input;
    Screen *screen;
    bool needClose;
    WCHAR lastKey;
    Timer deltaTimer;
    float keyTime;
    INPUT_RECORD inputRecord{};
    DWORD keyRead;
    // Buffers
    std::vector<std::string> strings;
    wchar_t *charBuffer;
    WORD *colorBuffer;
    // WordScreen variable
    Vector screenOffset;
};


#endif //ANOTHERWORD_ANOTHERWORD_H
