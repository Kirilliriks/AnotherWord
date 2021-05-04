//
// Created by KirilliriK on 04.05.2021.
//

#include "TextEditor.h"

TextEditor::TextEditor(AnotherWord *anotherWord) {
    this->anotherWord = anotherWord;
    screen = new Screen(120, 40);
    screenSize = screen->getMaxSize();
    screenOffset = Vector(0, 0);
    charBuffer = new wchar_t[screenSize.x * screenSize.y];
    colorBuffer = new WORD[screenSize.x * screenSize.y];

    setCursorPosition((Vector(0,0)));
}

void TextEditor::draw() {
    screen->draw(charBuffer, colorBuffer);
}

void TextEditor::preDraw() {
    int stringY;
    for (int y = 1; y < screenSize.y; y++) {
        stringY = screenOffset.y + y - 1;
        if (stringY >= strings.size()) break;
        for (int x = 0; x < screenSize.x; x++) {
            int xPos = x + screenOffset.x;
            if (xPos >= strings[stringY].length()) break;
            char ch = strings[stringY].at(xPos);
            if (ch == '\0') continue;
            charBuffer[x + y * screenSize.x] = ch;
            if (std::isdigit(ch)) colorBuffer[x + y * screenSize.x] = (WORD) Color::Orange | (WORD) BackgroundColor::Black;
            else colorBuffer[x + y * screenSize.x] = (WORD) Color::White | (WORD) BackgroundColor::Green;
        }
    }
}

void TextEditor::writeChar(const char ch) {
    if (alphabet.find_first_of(ch) == std::string::npos) return;
    if (anotherWord->state != AnotherWord::State::EDITOR) {
        if (anotherWord->state == AnotherWord::State::FIND_SUB_STRING)
            anotherWord->inputString.push_back(ch);
        else
            anotherWord->getFileName().push_back(ch);
        moveCursor(Vector(1, 0));
        return;
    }
    Vector position = getTextCursorPosition();
    if (position.y == -1) return;
    putChar(ch, position);
    moveCursor(Vector(1, 0));
}

void TextEditor::clearChar(){
    if (anotherWord->state != AnotherWord::State::EDITOR) {
        if (anotherWord->state == AnotherWord::State::FIND_SUB_STRING)
            if (anotherWord->inputString.length() >= 1) anotherWord->inputString.erase(anotherWord->inputString.length() - 1);
        else
            if (anotherWord->getFileName().length() >= 1) anotherWord->getFileName().erase(anotherWord->getFileName().length() - 1);
        moveCursor(Vector(-1, 0));
        return;
    }
    Vector position = getTextCursorPosition();
    if (position.y == -1) return;
    position.x--;
    if (position.x <= -1 && position.y > 0) {
        boolean needDelete = true;
        for (int i = position.y; i < strings.size(); i++) {
            if (strings.at(i).empty()) continue;
            if (strings.at(i).find_first_not_of(' ') == std::string::npos) continue;
            needDelete = false;
        }
        if (needDelete) {
            strings.erase(strings.begin() + position.y, strings.end());
        }
        if (position.y > strings.size()) {
            setCursorPosition(Vector(0, position.y)); // Не уменьшаем y на -1 т.к. координаты уже смешены
            return;
        }
        setCursorPosition(Vector(strings[position.y - 1].length(), position.y));  // При обращении к массиву уменьшаем , при установке курсора не уменьшаем y на -1 т.к. координаты уже смешены
        return;
    }
    moveCursor(Vector(-1, 0)); // 0
    if (position.y >= strings.size()) return; // 1
    std::string &str = strings[position.y];
    if (position.x >= str.length()) return;
    str.erase(position.x, 1);
}

void TextEditor::putChar(const char ch, Vector vector) {
    if (vector.x < 0 || vector.y < 0) return; // Предупреждение выхода за пределы экрана
    if (vector.y >= strings.size()) {
        int needPush = vector.y - (int)strings.size() + 1;
        for (int i = 0; i < needPush; i++) {
            strings.emplace_back("");
        }
    }

    if (vector.x >= strings[vector.y].length()) {
        strings[vector.y].insert(strings[vector.y].length(), vector.x - strings[vector.y].length(), ' ');
    }
    strings[vector.y].insert(vector.x, 1, ch);
}

void TextEditor::enterMove() {
    Vector cursorPosition = getCursorPosition();
    cursorPosition.y++;
    if (cursorPosition.y >= strings.size()){
        setCursorPosition(Vector(0, cursorPosition.y));
        return;
    }
    setCursorPosition(Vector(strings[cursorPosition.y].length(), cursorPosition.y));
}

void TextEditor::moveCursor(Vector moveVector) {
    setCursorPosition(screen->getCursorPosition() + moveVector);
}

void TextEditor::setCursorPosition(Vector vector) {
    // X
    if (vector.x > screenSize.x){
        screenOffset.x += 1;
    } else if (screenOffset.x > 0 && vector.x < 0){
        screenOffset.x -= 1;
        if (screenOffset.x < 0) screenOffset.x = 0;
    }

    // Y
    if (vector.y > screenSize.y){
        screenOffset.y += 1;
    } else if (screenOffset.y > 0 && vector.y < 0){
        screenOffset.y -= 1;
        if (screenOffset.y < 0) screenOffset.y = 0;
    }

    screen->setCursor(vector);
}

// Алгоритм Брезенхема
// (если честно он тут нафиг не нужен, по началу я планировал сделать мега красивый ворд, но увы)
// кстати один из моих любимыъх алгоритмов
void TextEditor::drawLine(const char ch, int x1, int y1, int x2, int y2, Color color, BackgroundColor backColor) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    drawChar(ch, x2, y2, color, backColor);
    while (x1 != x2 || y1 != y2) {
        drawChar(ch, x1, y1, color, backColor);
        int error2 = error * 2;
        if (error2 > -deltaY){
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX){
            error += deltaX;
            y1 += signY;
        }
    }
}

void TextEditor::drawString(std::string str, int x, int y, Color color, BackgroundColor backColor) {
    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i], x + i, y, color, backColor);
    }
}

void TextEditor::drawChar(char ch, int x, int y, Color color, BackgroundColor backColor) {
    drawChar(ch, Vector(x, y), color, backColor);
}

void TextEditor::drawChar(char ch, Vector vector, Color color, BackgroundColor backColor) {
    if (vector.x < 0 || vector.x > screenSize.x || vector.y < 0 || vector.y > screenSize.y) return;
    charBuffer[vector.x + vector.y * screenSize.x] = ch;
    colorBuffer[vector.x + vector.y * screenSize.x] = (WORD)color | (WORD)backColor;
}

Vector TextEditor::getCursorPosition() {
    return screen->getCursorPosition();
}

Vector TextEditor::getTextCursorPosition() {
    Vector result = screen->getCursorPosition() + screenOffset;
    result.y--;
    return result;
}

Vector &TextEditor::getScreenSize() {
    return screenSize;
}

std::vector<std::string> &TextEditor::getData() {
    return strings;
}

void TextEditor::clearBuffers() {
    for (int x = 0; x < screenSize.x; x++){
        for (int y = 0; y < screenSize.y; y++){
            charBuffer[x + y * screenSize.x] = ' ';
            colorBuffer[x + y * screenSize.x] = (WORD)Color::White | (WORD)BackgroundColor::Black;
        }
    }
}

TextEditor::~TextEditor() {
    delete screen;
    delete[] charBuffer;
    delete[] colorBuffer;
}
