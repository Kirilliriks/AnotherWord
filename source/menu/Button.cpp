//
// Created by KirilliriK on 28.04.2021.
//

#include "Button.h"

Button::Button(Vector position, std::string text, std::string groupName, AnotherWord *anotherWord) {
    this->anotherWord = anotherWord;
    this->position = position;
    this->text = text;
    this->groupName = groupName;
    this->active = false;
    this->clickTime = 0;
    this->toggled = false;
    this->color = BackgroundColor::LightBlue;
}

Button::Button(Vector position, std::string text, std::string groupName, AnotherWord *anotherWord, std::function<void(void)> function){
    this->anotherWord = anotherWord;
    this->position = position;
    this->text = text;
    this->groupName = groupName;
    this->function = function;
    this->active = false;
    this->clickTime = 0;
    this->toggled = false;
    this->color = BackgroundColor::LightBlue;
}

void Button::update(float deltaTime) {
    if (clickTime != 0) {
        clickTime -= deltaTime;
        if (clickTime < 0) clickTime = 0;
    }
    for (Button *button : buttons){
        if (!button->isActive()) continue;
        button->update(deltaTime);
    }
}

void Button::draw(AnotherWord *anotherWord) {
    anotherWord->getTextEditor().drawString(getText(), getPosition().x, getPosition().y, Color::White, toggled ? BackgroundColor::Red : color);
    for (Button *button : buttons){
        if (!button->isActive()) continue;
        button->draw(anotherWord);
    }
}

void Button::addButton(Button *button) {
    buttons.push_back(button);
}

void Button::toggleButtons() {
    toggled = !toggled;
    for (Button *button : buttons){
        button->setActive(toggled);
    }
}

void Button::setFunction(std::function<void(void)> function) {
    this->function = function;
}

void Button::onClick() {
    function(); // Выполняем функцию при клике

    clickTime = 100.0f; // Установка времени на повторный клик
}

bool Button::checkClick(Vector clickPosition) {
    if (!isActive()) return false;
    if (clickPosition.y == position.y && clickPosition.x >= position.x && clickPosition.x < (position.x + text.length())) { // Если нажатие по кнопке то выполняем
        if (clickTime == 0) {
            Button *lastButton = anotherWord->getLastButton();
            if (lastButton != nullptr && lastButton->getGroupName() != this->getGroupName() && lastButton->isToggled()) lastButton->toggleButtons();
            onClick();
        }
        return true;
    }
    // Если клик не по основной кнопке то проверяем подкнопки
    for (Button *button : buttons) {
        if (button->checkClick(clickPosition)) { // Если произошло нажатие по одному из подкнопок, то закрываем остальные подкнопки
            toggleButtons(); // Сворачиваем все дочерние кнопки
            return true;
        }
    }
    return false;
}

void Button::setText(std::string text) {
    this->text = text;
}

Vector Button::getPosition() {
    return position;
}

std::string Button::getText() {
    return text;
}

std::string Button::getGroupName() {
    return groupName;
}

void Button::setActive(bool active){
    this->active = active;
    if (!active){
        for (Button *button : buttons){
            button->setActive(active);
        }
    }
}

bool Button::isActive(){
    return active;
}

bool Button::isToggled() {
    return toggled;
}

Button::~Button() {
    buttons.clear();
}
