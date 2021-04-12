//
// Created by KirilliriK on 22.03.2021.
//

#include "Vector.h"

Vector::Vector(int x, int y) {
    this->x = x;
    this->y = y;
}

Vector::Vector() {
    this->x = 0;
    this->y = 0;
}

Vector Vector::operator + (Vector& vector){
    return Vector(x + vector.x, y + vector.y);
}

Vector Vector::operator - (Vector& vector) {
    return Vector(x - vector.x, y - vector.y);
}

void Vector::operator += (Vector& vector) {
    x += vector.x;
    y += vector.y;
}

void Vector::operator -= (Vector& vector) {
    x -= vector.x;
    y -= vector.y;
}

Vector::operator COORD() {
    return {(SHORT)x, (SHORT)y};
}
