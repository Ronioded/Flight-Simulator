// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#include <map>
#include <vector>
#include <string>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

/**
 * class TimeSeries - responsible on the data from csv file.
 */
class TimeSeries{
private:
    // fields of the object.
    vector<pair<string, vector<float>>> columnsVector;
    vector<vector<float>> rowsVector;
    vector<string> featuresVector;
    int numColumns;
    int numRows;

    /**
    * the function read the data from the CSV file and updating csvVector.
    * @param CSVfileName - the name of the file to read from.
    */
    void readFromCSV(const char* CSVfileName);

public:

	TimeSeries(const char* CSVfileName);

	TimeSeries(){
	    this->numRows = 0;
	    this->numColumns = 0;
	}
    vector<float> getColumn(int index) const;
	int getNumColumns() const;
	int getNumRows() const;
    string getColumnName(int index) const;
    vector<string> getFeaturesVector() const;
    vector<float> getRow(int index) const;
    int getIndexFeature(const string& feature) const;
    ~TimeSeries();
};

#endif /* TIMESERIES_H_ */
