//
// Created by KirilliriK on 28.04.2021.
//

#include "Button.h"

Button::Button(Vector position, std::string text) {
    this->position = position;
    this->text = text;
    this->active = false;
    this->clickTime = 0;
}

Button::Button(Vector position, std::string text, std::function<void(void)> function){
    this->position = position;
    this->text = text;
    this->function = function;
    this->active = false;
    this->clickTime = 0;
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
    anotherWord->drawString(getText(), getPosition().x, getPosition().y, Color::White, BackgroundColor::Green);
    for (Button *button : buttons){
        if (!button->isActive()) continue;
        button->draw(anotherWord);
    }
}

void Button::addButton(Button *button) {
    buttons.push_back(button);
}

void Button::toggleButtons() {
    for (Button *button : buttons){
        button->setActive(!button->isActive());
    }
}

void Button::setFunction(std::function<void(void)> function) {
    this->function = function;
}

void Button::onClick() {
    function();

    clickTime = 100.0f;
    clicked = !clicked;
}

bool Button::checkClick(Vector clickPosition) {
    if (!isActive()) return false;
    if (clickPosition.y == position.y && clickPosition.x >= position.x && clickPosition.x < (position.x + (text.length() - 1))){
        if (clickTime == 0) onClick();
        return true;
    }
    for (Button *button : buttons){
        if (button->checkClick(clickPosition)) return true;
    }
    return false;
}

Vector Button::getPosition() {
    return position;
}

void Button::setText(std::string text) {
    this->text = text;
}

std::string Button::getText() {
    return text;
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

Button::~Button() {
    buttons.clear();
}
