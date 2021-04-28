//
// Created by KirilliriK on 12.03.2021.
//

#ifndef ANOTHERWORD_ANOTHERWORD_H
#define ANOTHERWORD_ANOTHERWORD_H

#include <vector>
#include <string>
#include <istream>
#include <chrono>
#include <functional>
#include <windows.h>

#include "screen/Screen.h"
#include "util/Util.h"
#include "util/timer/Timer.h"
#include "conio.h"
#include "menu/Button.h"


class Button;
class AnotherWord {
public:
    enum State{
        EDITOR,
        OPEN_FILE
    };

    AnotherWord();
    ~AnotherWord();
    void start();
    void close();
    void update(float deltaTime);
    void draw(float deltaTime);
    // Методы для работы с файлами
    void loadFile(const std::string& path);
    void saveFile(const std::string& path);
    // Методы для отрисовки интерфейса программы (отрисованные символы не сохраняются в файле)
    void drawChar(char ch, int x, int y, Color color = Color::White, BackgroundColor backColor = BackgroundColor::Black);
    void drawChar(char ch, Vector vector, Color color = Color::White, BackgroundColor backColor = BackgroundColor::Black);
    void drawLine(char ch, int x1, int y1, int x2, int y2, Color color = Color::White, BackgroundColor backColor = BackgroundColor::Black);
    void drawString(std::string str, int x, int y, Color color = Color::White, BackgroundColor backColor = BackgroundColor::Black);
    // Логический обработчик перемешения мыши
    void moveCursor(Vector moveVector);
    void setCursorPosition(Vector vector);
    void enterMove();
    // Методы для очистки
    void clearBuffers();
private:
    // Методы программы
    void prepareOpenFile();
    // Методы для обработки ввода
    void handleInput(float deltaTime);
    void handleFileName();
    bool handleButton(float deltaTime); // Вернёт true если совершенно нажатие по кнопке меню
    void handleKeyboard(float deltaTime);
    void handleMouse(float deltaTime);
    // Методы для взаимодействия с текстом (вставленные символы сохраняются в файле)
    void writeChar(char ch);
    void clearChar();
    void putChar(char ch, Vector vector);
    void closeCurrent();
private:
    Vector screenSize;
    HANDLE input;
    Screen *screen;
    bool needClose;
    WCHAR lastKey;
    Timer deltaTimer;
    float keyTime;
    INPUT_RECORD inputRecord{};
    DWORD events;
    State state{};
    // Menu
    std::vector<Button*> buttons;
    std::string lastMessage;
    // File
    std::string fileName; // Название текущего файла
    // Buffers
    std::vector<std::string> strings;
    wchar_t *charBuffer;
    WORD *colorBuffer;
    // WordScreen variable
    Vector screenOffset;

    // CONSTANTS
    const Vector enterNamePosition = Vector(0, 2);
};

#endif //ANOTHERWORD_ANOTHERWORD_H
