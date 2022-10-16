//  [11/13/2009 Sasha]
#include "GeometryMath.h"

namespace NS_GeometryMath {

    //  [11/13/2009 Sasha]
    Line::Line(Point &p1, Point &p2) {
        Set(p1, p2);
    }

    //  [11/13/2009 Sasha]
    Line::Line(double _x1, double _y1, double _x2, double _y2) {
	    Point p1(_x1, _y1);
        Point p2(_x2, _y2);
	    Set(p1, p2);
    }

    //  [11/13/2009 Sasha]
    void Line::Set(Point &p1, Point &p2) {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        if(dx == 0) {
            vertical = true;
            a = p1.x;
        }
        else {
            vertical = false;
            k = dy / dx;
            a = p1.y - k * p1.x;
        }
    }

    //  [11/13/2009 Sasha]
    void Line::Set(double _x1, double _y1, double _x2, double _y2) {
	    Point p1(_x1, _y1);
	    Point p2(_x2, _y2);
        Set(p1, p2);
    }

    //  [11/13/2009 Sasha]
    double Line::GetValue(double x) {
        return (vertical == false) ? (a + k * x) : 0.0;;
    }

    //  [11/13/2009 Sasha]
    Point& Line::GetIntersection(Line &line) {
        static Point point;
        if(vertical) {
            point.Set(a, line.GetValue(a));
        }
        else if(line.vertical) {
            point.Set(line.a, GetValue(line.a));
        }
        else {
            double x0 = (a - line.a) / (line.k - k);
            double y0 = a + k * x0;
            point.Set(x0, y0);
        }
        return point;
    }

}