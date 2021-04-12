#include <iostream>

#include "AnotherWord.h"

AnotherWord *anotherWord;

int main() {
    anotherWord = new AnotherWord();
    anotherWord->start();
    getchar();
    delete anotherWord;
    return 0;
}
