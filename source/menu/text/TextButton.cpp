//
// Created by KirilliriK on 04.05.2021.
//

#include "TextButton.h"

TextButton::TextButton(Vector position, AnotherWord *anotherWord) : Button(position, "File", "File", anotherWord){
    color = BackgroundColor::Green;

    this->function = [this](){
        this->toggleButtons();
    };

    Button *childButton = new Button(Vector(position.x, 1), "Find substring", "File", anotherWord,
                                     [anotherWord](){
                                         anotherWord->preInputString(AnotherWord::State::FIND_SUB_STRING);
                                     });
    this->addButton(childButton);
}

TextButton::~TextButton() {
    Button::~Button();
}
