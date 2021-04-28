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
    Button(Vector position, std::string text);
    Button(Vector position, std::string text, std::function<void(void)> function);
    ~Button();
    void update(float deltaTime);
    void draw(AnotherWord *anotherWord);
    void addButton(Button *button);
    void toggleButtons();
    void setFunction(std::function<void(void)> function);
    virtual void onClick();
    bool checkClick(Vector clickPosition);
    Vector getPosition();
    void setText(std::string text);
    std::string getText();
    void setActive(bool active);
    bool isActive();
protected:
    bool active;
    bool clicked;
    float clickTime; // Время через которое кнопку можно нажать ещё раз
    Vector position;
    std::string text;
    std::function<void(void)> function; // Функция стандартно выполняемая при клике на кнопку

    std::vector<Button*> buttons;
};


#endif //ANOTHERWORD_BUTTON_H
