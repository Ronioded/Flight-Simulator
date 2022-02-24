// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#include "anomaly_detection_util.h"
#include <cmath>
#include <stdexcept>

// returns the simple average of the array.
float E(float* x, int size) {
    // handling division by zero or null pointer.
    if (size <= 0 || x == nullptr) {
        throw std::invalid_argument("division by zero or null pointer");
    }
    float sum = 0;
    // passing the array x and summing x[i].
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }
    return sum / (float) size;
}

// returns the variance of X and Y
float var(float* x , int size) {
    // handling division by zero or null pointer.
    if (size <= 0 || x == nullptr) {
        throw std::invalid_argument("division by zero or null pointer");
    }
    float sum = 0;
    // passing the array x and summing x[i]*x[i].
    for (int i = 0; i < size; i++) {
        sum += (x[i] * x[i]);
    }
    float e = E(x, size);
    return  sum / (float) size - e * e;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    // handling null pointer.
    if (y == nullptr || x == nullptr) {
        throw std::invalid_argument("null pointer");
    }
    float e_x = E(x, size);
    float e_y = E(y, size);
    auto* arr = new float[size];
    // passing the array and initialize the new array cov.
    for (int i = 0; i < size; i++) {
        arr[i] = (x[i] - e_x) * (y[i] - e_y);
    }
    float z = E(arr, size);
    delete[] arr;
    return z;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    return float(double(cov(x, y, size)) / (sqrt(double(var(x, size))) * sqrt(double((var(y, size))))));
}

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size) {
    // handling null pointer.
    if (points == nullptr) {
        throw std::invalid_argument("null pointer");
    }
    auto *x = new float[size];
    auto *y = new float[size];
    // passing the array of points and initializing new arrays of x and y values.
    for (int i = 0; i < size; i++) {
        x[i] = (*points[i]).x;
        y[i] = (*points[i]).y;
    }
    float a = cov(x, y, size) / var(x, size);
    float b = E(y, size) - a * E(x, size);

    // deleting the arrays
    delete[] x;
    delete[] y;

    // return new line.
    return {a, b};
}

// returns the deviation between point p and the line
float dev(Point p,Line l) {
    float abs = l.f(p.x) - p.y;
    // if abs is positive return abs, else return -abs.
    if (abs >= 0) {
        return abs;
    }
    return -abs;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
    // handling null pointer.
    if (points == nullptr) {
        throw std::invalid_argument("null pointer");
    }
    Line l = linear_reg(points, size);
    return dev(p, l);
}