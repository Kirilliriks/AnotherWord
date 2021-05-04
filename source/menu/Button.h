//
// Created by KirilliriK on 28.04.2021.
//

#ifndef ANOTHERWORD_BUTTON_H
#define ANOTHERWORD_BUTTON_H

#include <iostream>
#include <vector>
#include <functional>

#include "../screen/vector/Vector.h"
#include "../AnotherWord.h"

class AnotherWord;
class Button {
public:
    Button(Vector position, std::string text, std::string groupName, AnotherWord *anotherWord);
    Button(Vector position, std::string text, std::string groupName, AnotherWord *anotherWord, std::function<void(void)> function);
    ~Button();
    void update(float deltaTime);
    void draw(AnotherWord *anotherWord);
    void addButton(Button *button);
    void toggleButtons();
    void setFunction(std::function<void(void)> function);
    virtual void onClick();
    bool checkClick(Vector clickPosition);
    void setText(std::string text);
    Vector getPosition();
    std::string getText();
    std::string getGroupName();
    void setActive(bool active);
    bool isActive();
    bool isToggled();
protected:
    AnotherWord *anotherWord;

    Vector position;
    std::string text;
    std::string groupName; // Название группы кнопок к примеру "Menu"
    std::function<void(void)> function; // Функция стандартно выполняемая при клике на кнопку

    BackgroundColor color;
    bool active; // Кнопка активна
    bool toggled; // Если true то кнопка раскрыла свои подкнопки
    float clickTime; // Время через которое кнопку можно нажать ещё раз

    std::vector<Button*> buttons; // Подкнопки
};


#endif //ANOTHERWORD_BUTTON_H
