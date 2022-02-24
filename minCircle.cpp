// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#include "minCircle.h"
#include <vector>
#include <cmath>

// the function return if the point p is in the circle or not.
bool isInsideCircle(Circle circle, Point p) {
    return distance(circle.center, p) <= circle.radius;
}

// the function found the min circle that all the points will be inside him via the function findMinCircleRecursion.
Circle findMinCircle(Point** points,size_t size) {
    vector<Point> boundPoints = vector<Point>();
    return findMinCircleRecursion(points, boundPoints, size);
}

// the function found the min circle that all the points will be inside him.
Circle findMinCircleRecursion(Point** pointsVec, vector<Point> boundPoints, size_t size) {

    // stop condition of the recursion, if there is no points or there is 3 points in the boundaries,
    // it's enough to make a circle.
    if (size == 0 || boundPoints.size() == 3) {
        return createCircle(boundPoints);
    }

    // pick a random point from given vector and swap it to the end.
    unsigned int i = rand() % size;
    Point p = **(pointsVec + i);
    Point temp = **(pointsVec + size - 1);
    **(pointsVec + size - 1) = p;
    **(pointsVec + i) = temp;

    // get the circle of the points without p.
    Circle circle = findMinCircleRecursion(pointsVec, boundPoints, size - 1);

    // if p is in the circle return the circle.
    if (isInsideCircle(circle, p)) {
        return circle;
    }
    else {
        // else, add the point to boundPoints vector.
        boundPoints.push_back(p);
        return findMinCircleRecursion(pointsVec, boundPoints, size - 1);
    }
}

// the function create a circle from pointVec.
Circle createCircle(vector<Point> pointsVec) {
    // if there is no points, there is no circle so return base circle.
    if (pointsVec.empty()) {
        return {{0, 0}, 0};
    // else, if there is only 1 point, return circle that the center is the only point and radius is 0.
    } else if (pointsVec.size() == 1) {
        return {pointsVec[0], 0};
    // else, if there is 2 points, return the circle that the function circleFrom2Points return.
    } else if(pointsVec.size() == 2){
        return circleFrom2Points(pointsVec[0], pointsVec[1]);
    // else, there is 3 points, so return the circle that the function circleFrom3Points return.
    } else {
        return circleFrom3Points(pointsVec[0], pointsVec[1], pointsVec[2]);
    }
}

// create circle from 3 points.
Circle circleFrom3Points(const Point& a, const Point& b, const Point& c){
    float x1 = b.x - a.x;
    float y1 = b.y - a.y;
    float x2 = c.x - a.x;
    float y2 = c.y - a.y;

    // calculate the center of the new circle.
    float x = (y2 * (x1 * x1 + y1 * y1) - y1 * (x2 * x2 + y2 * y2)) / (2 * (x1 * y2 - y1 * x2));
    float y = (x1 * (x2 * x2 + y2 * y2) - x2 * (x1 * x1 + y1 * y1)) / (2 * (x1 * y2 - y1 * x2));
    Point center = {x + a.x, y + a.y};

    // create circle from the 3 points.
    return {center, distance(center, a)};
}

// create circle from 2 points.
Circle circleFrom2Points(const Point& a, const Point& b) {
    Point center = {(a.x + b.x) / float(2.0), (a.y + b.y) / float(2.0) };
    return {center,distance(a, b) / float(2.0)};
}

// return the distance between two points.
float distance(const Point& a, const Point& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}