//
// Created by KirilliriK on 22.03.2021.
//

#ifndef ANOTHERWORD_VECTOR_H
#define ANOTHERWORD_VECTOR_H

#include <windows.h>

class Vector {
public:
    int x;
    int y;
    Vector(int x, int y);
    Vector();
    Vector operator + (Vector& vector);
    Vector operator - (Vector& vector);
    void operator += (Vector& vector);
    void operator -= (Vector& vector);
    operator COORD();
};


#endif //ANOTHERWORD_VECTOR_H
