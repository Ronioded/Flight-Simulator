// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
    virtual bool checkIfException(Point p, const correlatedFeatures& cfObject, int i);
    virtual bool checkIfAddCorrelated(float maxCorrelation);
    virtual void addCircleOrLine(correlatedFeatures* cf, Point** pointsPtr, int numRows);
    virtual ~HybridAnomalyDetector();

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
