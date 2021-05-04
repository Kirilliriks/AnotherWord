//
// Created by KirilliriK on 04.05.2021.
//

#ifndef ANOTHERWORD_TEXTBUTTON_H
#define ANOTHERWORD_TEXTBUTTON_H


#include "../Button.h"

class TextButton: public Button {
public:
    TextButton(Vector position, AnotherWord *anotherWord);
    ~TextButton();
private:
};


#endif //ANOTHERWORD_TEXTBUTTON_H
