// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "minCircle.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

struct correlatedFeatures{
    explicit correlatedFeatures(Circle circle);

    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    Circle circle;
    float threshold;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;

public:
    float threshold = 0.9;
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    static float calcMaxDev(Point** points, Line lineReg, int size);
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    static Point** createPointPtr(vector<float> v1, vector<float> v2, int size);
    correlatedFeatures initializeCorrelatedFeatures(const TimeSeries& ts, int i, int j, float maxCorrelation,
                                                           int numRows, vector<float>& columnI);
    virtual bool checkIfException(Point p, const correlatedFeatures& cfObject, int i);
    virtual bool checkIfAddCorrelated(float maxCorrelation);
    virtual void addCircleOrLine(correlatedFeatures* cf, Point** pointsPtr, int numRows);
    static void destructPoints(Point** pointsPtr, int size);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
