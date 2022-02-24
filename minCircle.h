// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------



Circle findMinCircle(Point** points,size_t size);
Circle findMinCircleRecursion(Point** points, vector<Point> boundPoints, size_t size);
Circle createCircle(vector<Point> pointsVec);
bool isInsideCircle(Circle circle, Point p);
Circle circleFrom3Points(const Point& a, const Point& b, const Point& c);
Circle circleFrom2Points(const Point& a, const Point& b);
static float distance(const Point& a, const Point& b);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
