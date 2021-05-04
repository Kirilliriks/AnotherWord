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

#include "texteditor/TextEditor.h"
#include "screen/char/Color.h"
#include "util/Util.h"
#include "util/timer/Timer.h"
#include "conio.h"
#include "menu/Button.h"

class TextEditor;
class Button;
class AnotherWord {
public:
    enum State{
        EDITOR,
        OPEN_FILE,
        NEW_FILE,
        FIND_SUB_STRING
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
    // Методы программы
    void findSubstring(std::string subString);
    void preOpenFile(); // Подготовка к открытию нового файла
    void closeCurrent(); // Закрывает текущий файл(без сохранения)
    void preInputString(State state);
    // Методы для получения значений приватных полей
    Button *getLastButton(); // Закрывает последнее открытое меню
    std::string &getFileName(); // Возвращает название текущего файла
    TextEditor &getTextEditor(); // Возвращает текстовый редактор
public:
    State state{}; // Текущее состояние программы
    std::string inputString; // Вводимая строка (поиск и другие операции)
private:
    // Методы для обработки ввода
    void handleInput(float deltaTime);
    void handleStringInput();
    bool handleButton(float deltaTime); // Вернёт true если совершенно нажатие по кнопке меню
    void handleKeyboard(float deltaTime);
    void handleMouse(float deltaTime);
private:
    TextEditor *textEditor;
    HANDLE input;
    bool needClose;
    WCHAR lastKey;
    Timer deltaTimer;
    float keyTime;
    INPUT_RECORD inputRecord{};
    DWORD events;
    // Menu
    std::vector<Button*> buttons;
    std::string lastMessage;
    Button *lastButton; // Последнее открытое меню
    // File
    std::string fileName; // Название текущего файла

    // CONSTANTS
    const Vector enterNamePosition = Vector(0, 2);
};

#endif //ANOTHERWORD_ANOTHERWORD_H
