//  [11/13/2009 Sasha]
#pragma once

namespace NS_GeometryMath {

    struct Point {
        Point() : x(0), y(0) {}
        Point (double x, double y) : x(x), y(y) {}
        void Set(double x, double y) { this->x = x; this->y = y; }
        double x;
        double y;
    };

    class Line {

    public:
        Line(Point &p1, Point &p2);
        Line(double x1, double y1, double x2, double y2);

        void Set(Point &p1, Point &p2);
        void Set(double x1, double y1, double x2, double y2);
        double GetValue(double x);          // y = a + k * x;
        Point& GetIntersection(Line &line);

    private:
        // y = a + k * x;
        double a;
        double k;
        bool vertical;      // Если true, то прямая является вертикальной. При этом в точке x == a значение функции не определено
    };

}