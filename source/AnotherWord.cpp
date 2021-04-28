//
// Created by KirilliriK on 12.03.2021.
//

#include "AnotherWord.h"
#include "menu/main/FileButton.h"

AnotherWord::AnotherWord() {
    screen = new Screen(120, 40);
    screenSize = screen->getMaxSize();
    charBuffer = new wchar_t[screenSize.x * screenSize.y];
    colorBuffer = new WORD[screenSize.x * screenSize.y];
    input = GetStdHandle(STD_INPUT_HANDLE);
    needClose = false;
    lastKey = 0;
    keyTime = 0;
    events = 0;

    // Build menu
    auto *button = new FileButton(Vector(0, 0));
    auto *childButton = new Button(Vector(0, 1), "Open file",
                                   [button](){
                                       button->setText("Opened!");
                                   });
    button->addButton(childButton);
    childButton = new Button(Vector(0, 2), "Exit",
                                   [this](){
                                       this->close();
                                   });
    button->addButton(childButton);
    button->setActive(true);
    buttons.push_back(button);
    //
}

AnotherWord::~AnotherWord() {
    buttons.clear();
    delete screen;
    delete[] charBuffer;
    delete[] colorBuffer;
}

void AnotherWord::start() {
    while (!needClose){
        float deltaTime = deltaTimer.getDelta();
        update(deltaTime);
        draw(deltaTime);
    }
}

void AnotherWord::loadFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return;
    }
    std::string data;
    while (!in.eof()) {
        std::getline(in, data);
        strings.push_back(data);
    }
    in.close();
}

void AnotherWord::saveFile(const std::string& path){
    std::ofstream out(path);
    if (!out.is_open()){
        out.close();
        return;
    }
    for (auto &string : strings){
        out << string;
        out << std::endl;
    }
    out.close();
}

void AnotherWord::update(const float deltaTime) {
    for (Button *button : buttons){
        button->update(deltaTime);
    }
    handleInput(deltaTime);
}

void AnotherWord::draw(const float deltaTime) {
    clearBuffers();
    int stringY;
    for (int y = 1; y < screenSize.y; y++) {
        stringY = y - 1;
        if (stringY >= strings.size()) break;
        for (int x = 0; x < screenSize.x; x++) {
            int xPos = x + screenOffset.x;
            if (xPos >= strings[stringY].length()) break;
            char ch = strings[stringY].at(xPos);
            if (ch == '\0') continue;
            charBuffer[x + y * screenSize.x] = ch;
            colorBuffer[x + y * screenSize.x] = (WORD) Color::White | (WORD) BackgroundColor::Black;
        }
    }

    // DrawUI
    drawLine(' ', 0, 0, 119, 0, Color::White, BackgroundColor::Purple);
    std::string dTime = "Dt " + std::to_string(deltaTime) + " " + std::to_string(keyTime);
    drawString(dTime, screenSize.x - dTime.length() - 1,0, Color::White, BackgroundColor::Purple);

    for (Button *button : buttons){
        button->draw(this);
    }
    //

    screen->draw(charBuffer, colorBuffer);
}

void AnotherWord::handleInput(const float deltaTime) {
    if (keyTime != 0){
        keyTime -= deltaTime;
        if (keyTime < 0) {
            keyTime = 0;
            lastKey = 0;
        }
    }

    GetNumberOfConsoleInputEvents(input, &events);
    if (events == 0) return;
    if (ReadConsoleInput(input, &inputRecord, 1, &events)){
        if (handleButton(deltaTime)) return; // Если была нажата кнопка меню то не обрабатываем остальные входные данные
        switch(inputRecord.EventType) {
            case KEY_EVENT:
                handleKeyboard(deltaTime);
                break;
            case MOUSE_EVENT:
                handleMouse(deltaTime);
                break;
        }
        FlushConsoleInputBuffer(input);
    } else throw std::runtime_error("ReadConsoleInput error");
}

void AnotherWord::handleMouse(const float deltaTime) {
    if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
        Vector moveVector = Vector(inputRecord.Event.MouseEvent.dwMousePosition.X, inputRecord.Event.MouseEvent.dwMousePosition.Y);
        moveCursor(moveVector);
    }
}

bool AnotherWord::handleButton(float deltaTime) {
    if (inputRecord.Event.KeyEvent.wVirtualKeyCode != VK_RETURN) return false;
    for (Button *button : buttons){
        if (button->checkClick(screen->getCursorPos())) return true;
    }
    return false;
}

void AnotherWord::handleKeyboard(const float deltaTime) {
    boolean write = false;
    boolean pressed = false;
    wchar_t ch = ' ';
    Vector moveVector = Vector(0, 0);
    switch(inputRecord.Event.KeyEvent.wVirtualKeyCode){
        case VK_LEFT:
            if (lastKey == VK_LEFT) break;
            lastKey = VK_LEFT;
            pressed = true;
            if (GetAsyncKeyState(VK_MENU)){
                moveVector.x = -5;
            } else moveVector.x = -1;
            moveCursor(moveVector);
            break;
        case VK_RIGHT:
            if (lastKey == VK_RIGHT) break;
            lastKey = VK_RIGHT;
            pressed = true;
            if (GetAsyncKeyState(VK_MENU)){
                moveVector.x = 5;
            } else moveVector.x = 1;
            moveCursor(moveVector);
            break;
        case VK_UP:
            if (lastKey == VK_UP) break;
            lastKey = VK_UP;
            pressed = true;
            moveVector.y = -1;
            moveCursor(moveVector);
            break;
        case VK_DOWN:
            if (lastKey == VK_DOWN) break;
            lastKey = VK_DOWN;
            pressed = true;
            moveVector.y = 1;
            moveCursor(moveVector);
            break;
        case VK_BACK:
            if (lastKey == VK_BACK) break;
            lastKey = VK_BACK;
            pressed = true;
            clearChar();
            break;
        case VK_RETURN:
            moveCursor(Vector(0, 1));
            break;
        case VK_ESCAPE:
            close();
            break;
        default:
            if (!inputRecord.Event.KeyEvent.bKeyDown) break;
            if (lastKey == inputRecord.Event.KeyEvent.uChar.UnicodeChar) break;
            lastKey = ch = inputRecord.Event.KeyEvent.uChar.UnicodeChar;
            write = true;
            pressed = true;
            break;
    }
    if (pressed) keyTime = 100.0f;
    if (write) writeChar(ch);
}

void AnotherWord::writeChar(const char ch) {
    Vector position = screen->getCursorPos();
    if (position.y == 0) return;
    position.y--; // Чтобы обращаться к верной смещённой строчке текста относительно верхней строки-меню
    putChar(ch, position);
    moveCursor(Vector(1, 0));
}

void AnotherWord::clearChar(){
    if (screen->getCursorPos().y == 0) return;
    moveCursor(Vector(-1, 0));
    std::string &str = strings[screen->getCursorPos().y - 1];
    if (screen->getCursorPos().x >= str.length()) return;
    str.erase(screen->getCursorPos().x);
}

void AnotherWord::putChar(const char ch, Vector vector) {
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

// Алгоритм Брезенхема
void AnotherWord::drawLine(const char ch, int x1, int y1, int x2, int y2, Color color, BackgroundColor backColor) {
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

void AnotherWord::drawString(std::string str, int x, int y, Color color, BackgroundColor backColor) {
    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i], x + i, y, color, backColor);
    }
}

void AnotherWord::drawChar(char ch, int x, int y, Color color, BackgroundColor backColor) {
    drawChar(ch, Vector(x, y), color, backColor);
}

void AnotherWord::drawChar(char ch, Vector vector, Color color, BackgroundColor backColor) {
    if (vector.x < 0 || vector.x > screenSize.x || vector.y < 0 || vector.y > screenSize.y) return;
    charBuffer[vector.x + vector.y * screenSize.x] = ch;
    colorBuffer[vector.x + vector.y * screenSize.x] = (WORD)color | (WORD)backColor;
}

void AnotherWord::moveCursor(Vector moveVector){
    Vector endMove = moveVector;

    // X
    if (screen->getCursorPos().x + moveVector.x > screenSize.x){
        screenOffset.x += 1;
        endMove.x -= 1;
    } else if (screenOffset.x > 0 && screen->getCursorPos().x + moveVector.x < 0){
        screenOffset.x -= 1;
        endMove.x += 1;
    }

    // Y
    if (screen->getCursorPos().y + moveVector.y > screenSize.y){
        screenOffset.y += 1;
        endMove.y -= 1;
    } else if (screenOffset.y > 0 && screen->getCursorPos().y + moveVector.y < 0){
        screenOffset.y -= 1;
        endMove.y += 1;
    }

    screen->moveCursor(moveVector);
}

void AnotherWord::clearBuffers() {
    for (int x = 0; x < screenSize.x; x++){
        for (int y = 0; y < screenSize.y; y++){
            charBuffer[x + y * screenSize.x] = ' ';
            colorBuffer[x + y * screenSize.x] = (WORD)Color::White | (WORD)BackgroundColor::Black;
        }
    }
}

void AnotherWord::close() {
    //saveFile(fileName);
    exit(0);
}
