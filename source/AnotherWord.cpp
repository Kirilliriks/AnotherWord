//
// Created by KirilliriK on 12.03.2021.
//

#include "AnotherWord.h"
#include "menu/main/FileButton.h"
#include "menu/text/TextButton.h"

AnotherWord::AnotherWord() {
    textEditor = new TextEditor(this);
    needClose = false;
    keyTime = lastKey = events = 0;
    lastMessage = "Welcome!";
    state = State::EDITOR;
    lastButton = nullptr;
    input = GetStdHandle(STD_INPUT_HANDLE);
    fileName = inputString = "";

    // Build menu
    // File button
    Button *button = new FileButton(Vector(0, 0), this);
    button->setActive(true);
    buttons.push_back(button);
    // Text button
    button = new TextButton(Vector(5, 0), this);
    button->setActive(true);
    buttons.push_back(button);
    //
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

void AnotherWord::preOpenFile() {
    closeCurrent();
    preInputString(State::OPEN_FILE);
}

void AnotherWord::preInputString(State state){
    this->state = state;
    textEditor->setCursorPosition(enterNamePosition);
}

void AnotherWord::findSubstring(std::string subString) {
    boolean find = false;
    std::vector<std::string> &strings = textEditor->getData();
    for (std::string str : textEditor->getData()){
        int pos = str.find(subString);
        if (pos < 0) break;
        auto it = std::find(strings.begin(), strings.end(), str);
        if (it != strings.end()){
            auto idx = std::distance(strings.begin(), it);
            textEditor->setCursorPosition(Vector(pos, idx + 1));
            find = true;
            break;
        }
    }
    if (find) lastMessage = "Find finish";
    else lastMessage = "Not found";
    inputString.clear();
    state = State::EDITOR;
}

void AnotherWord::draw(const float deltaTime) {
    textEditor->clearBuffers();
    if (state == State::EDITOR) textEditor->preDraw();

    // DrawUI
    Vector screenSize = textEditor->getScreenSize();
    textEditor->drawLine(' ', 0, 0, screenSize.x - 1, 0, Color::White, BackgroundColor::Purple);
    textEditor->drawString(lastMessage, screenSize.x - lastMessage.length(), 0, Color::White, BackgroundColor::LightBlue);
    textEditor->drawString("Current: " + fileName, screenSize.x - lastMessage.length() - 10 - fileName.length(), 0, Color::White, BackgroundColor::Cyan);
    std::string stringSize = "Strings: ";
    stringSize += std::to_string(textEditor->getData().size());
    textEditor->drawString(stringSize, screenSize.x - lastMessage.length() - stringSize.length() - 11 - fileName.length(), 0, Color::White, BackgroundColor::Cyan);

    for (Button *button : buttons) {
        button->draw(this);
    }
    //

    if (state != State::EDITOR) {
        if (state == State::FIND_SUB_STRING) {
            textEditor->drawString("[Enter substring]", enterNamePosition.x, enterNamePosition.y - 1, Color::White,BackgroundColor::Blue);
            textEditor->drawString(inputString, enterNamePosition.x, enterNamePosition.y, Color::LightGreen, BackgroundColor::Black);
        } else {
            textEditor->drawString("[Enter file name]", enterNamePosition.x, enterNamePosition.y - 1, Color::White,BackgroundColor::Blue);
            textEditor->drawString(fileName, enterNamePosition.x, enterNamePosition.y, Color::LightGreen, BackgroundColor::Black);
        }

    }

    textEditor->draw();
}

void AnotherWord::handleInput(const float deltaTime) {
    if (keyTime != 0){
        keyTime -= deltaTime;
        if (keyTime <= 0) keyTime = lastKey = 0;
    }

    GetNumberOfConsoleInputEvents(input, &events);
    if (events == 0) return;
    if (state == State::EDITOR) handleMouse(deltaTime);
    if (ReadConsoleInput(input, &inputRecord, 1, &events)){
        if (handleButton(deltaTime)) return; // Если была нажата кнопка меню то не обрабатываем остальные входные данные
        if (lastKey == inputRecord.Event.KeyEvent.wVirtualKeyCode) return;
        if (state != State::EDITOR){
            handleStringInput();
            return;
        }
        switch(inputRecord.EventType) {
            case KEY_EVENT:
                handleKeyboard(deltaTime);
                break;
        }
    } else throw std::runtime_error("ReadConsoleInput error");
}

void AnotherWord::handleMouse(const float deltaTime) {
    if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
        lastKey = 0;
        Vector moveVector = Vector(inputRecord.Event.MouseEvent.dwMousePosition.X, inputRecord.Event.MouseEvent.dwMousePosition.Y);
        textEditor->setCursorPosition(moveVector);
        handleButton(deltaTime);
    }
}

bool AnotherWord::handleButton(float deltaTime) {
    if (lastKey == VK_RETURN) return false;
    if (inputRecord.Event.KeyEvent.wVirtualKeyCode != VK_RETURN && inputRecord.Event.MouseEvent.dwButtonState != FROM_LEFT_1ST_BUTTON_PRESSED) return false;
    for (Button *button : buttons){
        if (button->checkClick(textEditor->getCursorPosition())) {
            lastButton = button;
            lastKey = VK_RETURN;
            return true;
        }
    }
    return false;
}

void AnotherWord::handleStringInput() {
    switch(inputRecord.Event.KeyEvent.wVirtualKeyCode) {
        case VK_RETURN: // Пользователь закончил ввод строки
            switch(state){
                case FIND_SUB_STRING:
                    findSubstring(inputString);
                    break;
                case OPEN_FILE:
                    loadFile(fileName);
                    break;
                case NEW_FILE:
                    saveFile(fileName);
                    break;
            }
            state = State::EDITOR;
            break;
        case VK_BACK:
            textEditor->clearChar();
            break;
        default:
            if (!inputRecord.Event.KeyEvent.bKeyDown) return;
            textEditor->writeChar(inputRecord.Event.KeyEvent.uChar.UnicodeChar);
            break;
    }
    lastKey = inputRecord.Event.KeyEvent.wVirtualKeyCode;
    keyTime = 160.0f;
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
            textEditor->clearChar();
            break;
        case VK_RETURN:
            textEditor->enterMove();
            break;
        case VK_ESCAPE:
            close();
            return;
        default:
            textEditor->writeChar(inputRecord.Event.KeyEvent.uChar.UnicodeChar);
            break;
    }
    if (moveVector.x != 0 || moveVector.y != 0) textEditor->moveCursor(moveVector);
    lastKey = inputRecord.Event.KeyEvent.wVirtualKeyCode;
    keyTime = 80.0f;
}

std::string &AnotherWord::getFileName() {
    return fileName;
}

Button *AnotherWord::getLastButton() {
    return lastButton;
}

TextEditor &AnotherWord::getTextEditor() {
    return *textEditor;
}

void AnotherWord::loadFile(const std::string& fileName) {
    std::ifstream in(fileName);
    if (!in.is_open()) {
        lastMessage = "File not found";
        return;
    }
    std::string data;
    while (!in.eof()) {
        std::getline(in, data);
        textEditor->getData().push_back(data);
    }
    in.close();
    lastMessage = "File open";
}

void AnotherWord::saveFile(const std::string& fileName){
    if (fileName.empty()) {
        lastMessage = "Enter new file name";
        state = State::NEW_FILE;
        textEditor->setCursorPosition(enterNamePosition);
        return;
    }

    std::ofstream out(fileName);
    if (!out.is_open()){
        lastMessage = "Can't save file";
        return;
    }
    for (auto &string: textEditor->getData()){
        out << string;
        out << std::endl;
    }
    out.close();
    lastMessage = "File saved";
}

void AnotherWord::closeCurrent() {
    textEditor->getData().clear();
    fileName.clear();
    lastMessage = "File closed";
}

void AnotherWord::close() {
    closeCurrent();
    lastMessage = "Goodbye!";
    exit(0);
}

AnotherWord::~AnotherWord() {
    buttons.clear();
    delete lastButton;
    delete textEditor;
}
