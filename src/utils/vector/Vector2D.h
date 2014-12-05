#ifndef VECTOR_2D_H
#define VECTOR_2D_H

/**
 * @file Vector2D.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 *
 * @class Vector2D
 */

//TODO mathematique function add, sub, ...
//TODO verifier les operations arithmetic
class Vector2D {
public:
    Vector2D();
    Vector2D(double x, double y);
    ~Vector2D();

    // ##### GETTERS & SETTERS #####

    double getX() const;
    void setX(double x);
    double getY() const;
    void setY(double y);

    Vector2D operator+(Vector2D const &rhs);
    void operator+=(Vector2D const &rhs);

    Vector2D operator-(Vector2D const &rhs);
    void operator-=(Vector2D const &rhs);

private:
    double m_x;
    double m_y;
};

#endif /* VECTOR_2D_H */
