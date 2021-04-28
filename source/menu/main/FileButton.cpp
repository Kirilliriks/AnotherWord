//
// Created by KirilliriK on 28.04.2021.
//

#include "FileButton.h"

FileButton::FileButton(Vector position) : Button(position, "Menu"){
    function = [this](){
        this->toggleButtons();
    };
}

FileButton::~FileButton() {
    Button::~Button();
}
