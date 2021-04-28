//
// Created by KirilliriK on 12.03.2021.
//

#include "AnotherWord.h"
#include "menu/main/FileButton.h"

AnotherWord::AnotherWord() {
    screen = new Screen(120, 40);
    screenSize = screen->getMaxSize();
    screenOffset = Vector(0, 0);
    charBuffer = new wchar_t[screenSize.x * screenSize.y];
    colorBuffer = new WORD[screenSize.x * screenSize.y];
    input = GetStdHandle(STD_INPUT_HANDLE);
    needClose = false;
    lastKey = 0;
    keyTime = 0;
    events = 0;
    lastMessage = "Welcome!";
    state = State::EDITOR;

    // Build menu
    auto *button = new FileButton(Vector(0, 0));
    Button *childButton;
    childButton = new Button(Vector(0, 1), "Open file",
                 [this](){
                     this->prepareOpenFile();
                 });
    button->addButton(childButton);
    childButton = new Button(Vector(0, 2), "Save file",
                             [this](){
                                 this->saveFile(fileName);
                             });
    button->addButton(childButton);
    childButton = new Button(Vector(0, 3), "Close file",
                             [this](){
                                 this->closeCurrent();
                             });
    button->addButton(childButton);
    childButton = new Button(Vector(0, 4), "Exit",
                                   [this](){
                                       this->close();
                                   });
    button->addButton(childButton);
    button->setActive(true);
    buttons.push_back(button);
    //
    setCursorPosition((Vector(0,0)));
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

void AnotherWord::update(const float deltaTime) {
    for (Button *button : buttons){
        button->update(deltaTime);
    }

    handleInput(deltaTime);
    FlushConsoleInputBuffer(input);
}

void AnotherWord::prepareOpenFile() {
    closeCurrent();
    state = State::OPEN_FILE;
    setCursorPosition(enterNamePosition);
}


void AnotherWord::draw(const float deltaTime) {
    clearBuffers();
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
            colorBuffer[x + y * screenSize.x] = (WORD) Color::White | (WORD) BackgroundColor::Black;
        }
    }

    // DrawUI
    drawLine(' ', 0, 0, screenSize.x - 1, 0, Color::White, BackgroundColor::Purple);
    drawString(lastMessage, screenSize.x - lastMessage.length(), 0, Color::White, BackgroundColor::LightBlue);

    for (Button *button : buttons){
        button->draw(this);
    }
    //

    if (state == State::OPEN_FILE){
        drawString(fileName, enterNamePosition.x, enterNamePosition.y, Color::White, BackgroundColor::Black);
    }

    screen->draw(charBuffer, colorBuffer);
}

void AnotherWord::handleInput(const float deltaTime) {
    if (keyTime != 0){
        keyTime -= deltaTime;
        if (keyTime <= 0) {
            keyTime = 0;
            lastKey = 0;
        }
    }

    GetNumberOfConsoleInputEvents(input, &events);
    if (events == 0) return;
    if (ReadConsoleInput(input, &inputRecord, 1, &events)){
        if (handleButton(deltaTime)) return; // Если была нажата кнопка меню то не обрабатываем остальные входные данные
        if (lastKey == inputRecord.Event.KeyEvent.wVirtualKeyCode) return;
        if (state == State::OPEN_FILE){
            handleFileName();
            return;
        }
        switch(inputRecord.EventType) {
            case KEY_EVENT:
                handleKeyboard(deltaTime);
                break;
            case MOUSE_EVENT:
                handleMouse(deltaTime);
                break;
        }
    } else throw std::runtime_error("ReadConsoleInput error");
}

void AnotherWord::handleFileName() {
    switch(inputRecord.Event.KeyEvent.wVirtualKeyCode) {
        case VK_RETURN: // Пользователь закончил ввод имени файла
            state = State::EDITOR;
            loadFile(fileName);
            break;
        case VK_BACK:
            clearChar();
            break;
        default:
            if (!inputRecord.Event.KeyEvent.bKeyDown) return;
            lastKey = inputRecord.Event.KeyEvent.wVirtualKeyCode;
            keyTime = 80.0f;
            writeChar(inputRecord.Event.KeyEvent.uChar.UnicodeChar);
    }
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
        if (button->checkClick(screen->getCursorPos())) {
            lastKey = VK_RETURN;
            return true;
        }
    }
    return false;
}

void AnotherWord::handleKeyboard(const float deltaTime) {
    Vector moveVector = Vector(0, 0);
    switch(inputRecord.Event.KeyEvent.wVirtualKeyCode){
        case VK_LEFT:
            if (GetAsyncKeyState(VK_MENU)){
                moveVector.x = -5;
            } else moveVector.x = -1;
            break;
        case VK_RIGHT:
            if (GetAsyncKeyState(VK_MENU)){
                moveVector.x = 5;
            } else moveVector.x = 1;
            break;
        case VK_UP:
            moveVector.y = -1;
            break;
        case VK_DOWN:
            moveVector.y = 1;
            break;
        case VK_BACK:
            clearChar();
            break;
        case VK_RETURN:
            enterMove();
            break;
        case VK_ESCAPE:
            close();
            return;
        default:
            if (!inputRecord.Event.KeyEvent.bKeyDown) return;
            if (alphabet.find_first_of(inputRecord.Event.KeyEvent.uChar.UnicodeChar) != std::string::npos) writeChar(inputRecord.Event.KeyEvent.uChar.UnicodeChar);
            break;
    }
    if (moveVector.x != 0 || moveVector.y != 0) moveCursor(moveVector);
    lastKey = inputRecord.Event.KeyEvent.wVirtualKeyCode;
    keyTime = 80.0f;
}

Vector AnotherWord::getCursorPos() {
    Vector result = screen->getCursorPos() + screenOffset;
    result.y--;
    return result;
}

void AnotherWord::writeChar(const char ch) {
    if (state == State::OPEN_FILE){
        fileName.push_back(ch);
        moveCursor(Vector(1, 0));
        return;
    }
    Vector position = getCursorPos();
    if (position.y == -1) return;
    putChar(ch, position);
    moveCursor(Vector(1, 0));
}

void AnotherWord::clearChar(){
    if (state == State::OPEN_FILE){
        if (fileName.length() >= 1) fileName.erase(fileName.length() - 1);
        moveCursor(Vector(-1, 0));
        return;
    }
    Vector position = getCursorPos();
    if (position.y == -1) return;
    position.x--;
    moveCursor(Vector(-1, 0));
    if (position.y >= strings.size()) return;
    std::string &str = strings[position.y];
    if (position.x >= str.length()) return;
    str.erase(position.x, 1);
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

void AnotherWord::enterMove() {
    Vector cursorPosition = getCursorPos();
    if (cursorPosition.y >= strings.size()){
        setCursorPosition(Vector(0, cursorPosition.y));
        return;
    }
    setCursorPosition(Vector(strings[cursorPosition.y].length(), cursorPosition.y + 1));
}

void AnotherWord::moveCursor(Vector moveVector) {
    setCursorPosition(screen->getCursorPos() + moveVector);
}

void AnotherWord::setCursorPosition(Vector vector) {
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

void AnotherWord::clearBuffers() {
    for (int x = 0; x < screenSize.x; x++){
        for (int y = 0; y < screenSize.y; y++){
            charBuffer[x + y * screenSize.x] = ' ';
            colorBuffer[x + y * screenSize.x] = (WORD)Color::White | (WORD)BackgroundColor::Black;
        }
    }
}

void AnotherWord::loadFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        lastMessage = "File not found";
        return;
    }
    std::string data;
    while (!in.eof()) {
        std::getline(in, data);
        strings.push_back(data);
    }
    in.close();
    lastMessage = "File open";
}

void AnotherWord::saveFile(const std::string& path){
    if (fileName.empty()) return;

    std::ofstream out(path);
    if (!out.is_open()){
        lastMessage = "Can't save file";
        return;
    }
    for (auto &string : strings){
        out << string;
        out << std::endl;
    }
    out.close();
    lastMessage = "File saved";
}

void AnotherWord::closeCurrent() {
    saveFile(fileName);
    strings.clear();
    fileName.clear();
    lastMessage = "File closed";
}

void AnotherWord::close() {
    closeCurrent();
    lastMessage = "Goodbye!";
    exit(0);
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
