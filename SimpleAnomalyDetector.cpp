// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#include "SimpleAnomalyDetector.h"
#include <cmath>

/**
 * the constructor is initializing the fields.
 */
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->cf = vector<correlatedFeatures> ();
}

/**
 * the destructor is clearing the vectors in the field.
 */
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    this->cf.clear();
}

/**
 * the function find the correlated columns and save them on the cf vector.
 * @param ts - the time series to learn from.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    int numOfColumns = ts.getNumColumns();
    int numOfRows = ts.getNumRows();

    // loop through the columns
    for (int i = 0; i < numOfColumns; i++) {
        vector<float> iColumn = ts.getColumn(i);

        // initialize the max correlation.
        float maxCorrelation = 0;
        int column = -1;
        // moving the columns after the column i.
        for (int j = i + 1; j < numOfColumns; j++) {
            vector<float> jColumn = ts.getColumn(j);
            float p = fabs(pearson(reinterpret_cast<float *>(&iColumn[0]),
                                   reinterpret_cast<float *>(&jColumn[0]), numOfRows));
            // if the correlation of the columns is bigger then the max correlation,update it to be the max correlation.
            if (p > maxCorrelation) {
                maxCorrelation = p;
                column = j;
            }
        }
        // i and j are correlated featured if the function checkIfAddCorrelated return true.
        if ((column != -1) && checkIfAddCorrelated(maxCorrelation)) {
            // create an object of the correlated features.
            correlatedFeatures correlatedF = initializeCorrelatedFeatures(ts, i, column, maxCorrelation, numOfRows,
                                                                          iColumn);
            // push to the vector of correlated features.
            this->cf.push_back(correlatedF);
        }
    }
}

/**
 * the function initialize a correlated features object.
 * @param ts  - time series.
 * @param i - the index of column i.
 * @param j - the index of column j.
 * @param maxCorrelation - max correlation.
 * @param numRows - size of vector columnI.
 * @param columnI - the columnI in order to update the points from the correlated features.
 * @return an object of the correlated features.
 */
correlatedFeatures SimpleAnomalyDetector::initializeCorrelatedFeatures(const TimeSeries& ts, int i, int j,
                                                                       float maxCorrelation, int numRows,
                                                                       vector<float>& columnI) {
    // create new correlated features object.
    correlatedFeatures correlatedF(Circle(Point(0,0), 0));
    // define his members
    correlatedF.feature1 = ts.getColumnName(i);
    correlatedF.feature2 = ts.getColumnName(j);
    correlatedF.corrlation = maxCorrelation;

    vector<float> columnJ = ts.getColumn(j);
    // create Point** of the points from the correlated feature and create linear reg.
    Point** pointsPtr = createPointPtr(columnI, columnJ, numRows);
    // add circle or line and threshold according to the function addCircleOrLine.
    addCircleOrLine(&correlatedF, pointsPtr, numRows);
    // delete the memory allocated for the points pointer.
    destructPoints(pointsPtr, numRows);
    // return correlatedFeatures.
    return correlatedF;
}

/**
 * the function delete the allocated memory of the pointsPtr.
 * @param pointsPtr - the pointer to delete allocated memory.
 * @param size - the size of pointsPtr.
 */
void SimpleAnomalyDetector::destructPoints(Point** pointsPtr, int size) {
    // moving on the allocated memory and free each Point*
    for (int i = 0 ; i < size; i++) {
        delete pointsPtr[i];
    }
    // delete pointsPtr.
    delete pointsPtr;
}

/**
 * the function calculate the max dev, check the devs of all the points from the line and return the max
 * @param points - the array of points from the correlated features values.
 * @param lineReg - the line of the correlated features.
 * @param size - the size of points.
 * @return the maxDev.
 */
float SimpleAnomalyDetector::calcMaxDev(Point** points, Line lineReg, int size) {
    // calculate the max dev, and push him to the deviation vector.
    float maxDev = 0;
    // loop through the points of the array, calculate the dev and save the maximum.
    for (int i = 0; i < size ; i++) {
        float d = dev(*points[i], lineReg);
        if (d > maxDev) {
            maxDev = d;
        }
    }
    float preventFalseAlarms = 1.1;
    // push the max dev to the vector of max devs with the c
    return preventFalseAlarms * maxDev;
}

/**
 * the function create array of points with values of the given vectors.
 * @param v1 - the first vector.
 * @param v2 - the second vector.
 * @param size 0 the sizes of the vector.
 * @return the array of points from the vectors.
 */
Point** SimpleAnomalyDetector::createPointPtr(vector<float> v1, vector<float> v2, int size) {
    auto ptr = new Point*[size];
    // loop through the vectors and save their values in pointer to point which each point is (v1, v2).
    for (int i = 0; i < size; i++) {
        auto pointPtr = new Point(v1.at(i), v2.at(i));
        ptr[i] = pointPtr;
    }
    return ptr;
}

/**
 * the function detect the errors and return a vector of Anomaly Report.
 * @param ts - the time series.
 * @return a vector of Anomaly Report.
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    // initialize reportVector,
    vector<AnomalyReport> reportVector = {};
    int numRows = ts.getNumRows();

    // passing all the correlated features.
    for (const correlatedFeatures &cfObject : this->cf) {
        // passing the rows from ts.
        for (int i = 0; i < numRows; i++) {
            // rowI is the row on index i.
            vector<float> rowI = ts.getRow(i);
            // p is the point that the x value is rowI on index of feature1, and y value is rowI on index of feature2.
            Point p = Point(rowI.at(ts.getIndexFeature(cfObject.feature1)),
                            rowI.at(ts.getIndexFeature(cfObject.feature2)));
            // if the function checkIfException return true, add the exception to the report vector.
            if (checkIfException(p, cfObject, i)) {
                reportVector.emplace_back((cfObject.feature1 + "-"
                                           + cfObject.feature2), i + 1);
            }
        }
    }
    return reportVector;
}

/**
 * the method return true if there is exception and false other.
 * @param p - the point to check exception.
 * @param cfObject - the correlated feature to check.
 * @param i - the index.
 * @return return true if there is exception and false other.
 */
bool SimpleAnomalyDetector::checkIfException(Point p, const correlatedFeatures& cfObject, int i) {
    // check the dev from p to the linear_reg of the cfObject.
    float devP = ::dev(p, cfObject.lin_reg);
    // if devP is bigger then the maxDev,return true. else, return false.
    if (devP > cfObject.threshold) {
        return true;
    } else {
        return false;
    }
}

/**
 * the method return the if the correlation is above the threshold of both simple and hybrid anomaly detector.
 * @param maxCorrelation - the correlation to check.
 * @return true if we need to add correlated or false otherwise.
 */
bool SimpleAnomalyDetector::checkIfAddCorrelated(float maxCorrelation) {
    return (maxCorrelation >= this->threshold);
}

/**
 * the method update the correlatedF circle or line and threshold.
 * @param correlatedF - a pointer to correlatedF in order to update him.
 * @param pointsPtr - the points pointer.
 * @param numRows - the size of the array.
 */
void SimpleAnomalyDetector::addCircleOrLine(correlatedFeatures* correlatedF, Point** pointsPtr, int numRows) {
    correlatedF->lin_reg = linear_reg(pointsPtr, numRows);
    correlatedF->threshold = calcMaxDev(pointsPtr, correlatedF->lin_reg, numRows);
}

/**
 * constructor of correlatedFeatures with base values.
 * @param circle - the circle to add to the correlatedFeature.
 */
correlatedFeatures::correlatedFeatures(Circle circle) : circle(circle) {
    this->threshold = 0;
    this->lin_reg = Line();
    this->feature1 = "";
    this->feature2 = "";
    this->corrlation = 0;
    this->circle = circle;
}