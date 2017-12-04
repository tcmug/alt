#ifndef _EDITOR_POINT_H
#define _EDITOR_POINT_H

class Point {
    public:

        Point(): _x(0), _y(0) {}
        Point(float x, float y): _x(x), _y(y) {}

        Point operator - (const Point &other) const {
            return Point(_x - other._x, _y - other._y);
        }

        Point operator + (const Point &other) const {
            return Point(_x + other._x, _y + other._y);
        }

        Point operator * (const Point &other) const {
            return Point(_x * other._x, _y * other._y);
        }

        Point operator / (const Point &other) const {
            return Point(_x / other._x, _y / other._y);
        }

        void set(const float &x, const float &y) {
            _x = x;
            _y = y;
        }

        float _x, _y;

};

#endif
