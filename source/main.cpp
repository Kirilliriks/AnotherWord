#include <iostream>

#include "AnotherWord.h"

AnotherWord *anotherWord;

int main() {
    anotherWord = new AnotherWord();
    anotherWord->start();
    delete anotherWord;
    return 0;
}
