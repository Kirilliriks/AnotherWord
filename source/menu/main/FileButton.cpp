//
// Created by KirilliriK on 28.04.2021.
//

#include "FileButton.h"

FileButton::FileButton(Vector position, AnotherWord *anotherWord) : Button(position, "Menu", "Menu", anotherWord){
    color = BackgroundColor::Green;

    this->function = [this](){
        this->toggleButtons();
    };

    Button *childButton = new Button(Vector(position.x, 1), "Open file", "Menu", anotherWord,
                             [anotherWord](){
                                 anotherWord->preOpenFile();
                             });
    this->addButton(childButton);
    childButton = new Button(Vector(position.x, 2), "Save file", "Menu", anotherWord,
                             [anotherWord](){
                                 anotherWord->saveFile(anotherWord->getFileName());
                             });
    this->addButton(childButton);
    childButton = new Button(Vector(position.x, 3), "Close file", "Menu", anotherWord,
                             [anotherWord](){
                                 anotherWord->closeCurrent();
                             });
    this->addButton(childButton);
    childButton = new Button(Vector(position.x, 4), "Exit", "Menu", anotherWord,
                             [anotherWord](){
                                 anotherWord->close();
                             });
    this->addButton(childButton);
}

FileButton::~FileButton() {
    Button::~Button();
}
