//
// Created by KirilliriK on 04.05.2021.
//

#ifndef ANOTHERWORD_TEXTEDITOR_H
#define ANOTHERWORD_TEXTEDITOR_H
#include "../AnotherWord.h"
#include "../screen/Screen.h"
#include "../screen/char/Color.h"
#include "../screen/vector/Vector.h"

class AnotherWord;
class TextEditor {
public:
    TextEditor(AnotherWord *anotherWord);
    ~TextEditor();
    void update();
    void preDraw();
    void draw();
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
    // Публичные методы взаимодействия с текстом
    void writeChar(char ch);
    void clearChar();
    // Методы для получения значений приватных полей
    Vector &getScreenSize();
    std::vector<std::string> &getData(); // Возвращает все строчки редактора
    // Методы взаимодействия с экраном
    Vector getCursorPosition(); // Возвращает оригинальное положение курсора
private:
    // Методы взаимодействия с экраном
    Vector getTextCursorPosition(); // Возвращается смешённый относительно текста положение курсора
    // Методы для взаимодействия с текстом (вставленные символы сохраняются в файле)
    void putChar(char ch, Vector vector);
private:
    AnotherWord *anotherWord;
    Screen *screen;
    Vector screenSize;
    // Buffers
    std::vector<std::string> strings;
    wchar_t *charBuffer;
    WORD *colorBuffer;
    // Screen offset
    Vector screenOffset;

    //CONSTANTS
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,./\?;:'1234567890-_=+`~!@#$%^&*()[]{}";
};


#endif //ANOTHERWORD_TEXTEDITOR_H
