// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#include "HybridAnomalyDetector.h"

/**
 * constructor of HybridAnomalyDetector call the constructor of SimpleAnomalyDetector.
 */
HybridAnomalyDetector::HybridAnomalyDetector() : SimpleAnomalyDetector(){}

/**
 * deconstructor of HybridAnomalyDetector.
 */
HybridAnomalyDetector::~HybridAnomalyDetector() {}

/**
 * the method return true if there is exception and false other.
 * @param p - the point to check exception.
 * @param cfObject - the correlated feature to check.
 * @param i - the index.
 * @return return true if there is exception and false other.
 */
bool HybridAnomalyDetector::checkIfException(Point p, const correlatedFeatures& cfObject, int i) {
    // if the correlation is above or equal 0.9, this is like the simple anomaly detector so calling it's function.
    if (cfObject.corrlation >= this->threshold) {
        return SimpleAnomalyDetector::checkIfException(p, cfObject, i);
        // else, if the point is outside the circle, there is exception so return true, else return false.
    } else {
        if (isInsideCircle(cfObject.circle, p)) {
            return false;
        } else {
            return true;
        }
    }
}

/**
 * the method return the if the correlation is above the threshold of both simple and hybrid anomaly detector.
 * @param maxCorrelation - the correlation to check.
 * @return true if we need to add correlated or false otherwise.
 */
bool HybridAnomalyDetector::checkIfAddCorrelated(float maxCorrelation) {
    return (maxCorrelation > 0.5);
}

/**
 * the method update the correlatedF circle or line and threshold.
 * @param correlatedF - a pointer to correlatedF in order to update him.
 * @param pointsPtr - the points pointer.
 * @param numRows - the size of the array.
 */
void HybridAnomalyDetector::addCircleOrLine(correlatedFeatures* correlatedF, Point** pointsPtr, int numRows) {
    // if the correlation is above or equal 0.9, this is like the simple anomaly detector so calling it's function.
    if (correlatedF->corrlation >= this->threshold) {
        SimpleAnomalyDetector::addCircleOrLine(correlatedF, pointsPtr, numRows);
        // else, add the min circle to the correlatedF, and update the threshold as the radius.
    } else {
        correlatedF->circle = findMinCircle(pointsPtr, numRows);
        correlatedF->circle.radius =  correlatedF->circle.radius * float(1.1);
        correlatedF->threshold = correlatedF->circle.radius;
    }
}