// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#include "timeseries.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

/**
 * constructor of TimeSeries. reading the CSV file and updating the fields.
 * @param CSVfileName - the name of the file to read from.
 */
TimeSeries::TimeSeries(const char* CSVfileName) {
    this->columnsVector = vector<pair<string, vector<float>>> {};
    this->rowsVector = vector<vector<float>> {};
    this->featuresVector = vector<string> {};
    readFromCSV(CSVfileName);
    this->numColumns = this->columnsVector.size();
    this->numRows = this->rowsVector.size();
}

/**
 * the function read the data from the CSV file and updating rowsVector,columnsVector and featuresVector.
 * @param CSVfileName - the name of the file to read from.
 */
void TimeSeries::readFromCSV(const char* CSVfileName) {
    // create a stream and make sure the file is open.
    ifstream csvFile(CSVfileName);
    if(!csvFile.is_open()) {
        throw runtime_error("Could not open file");
    }
    string line, columnName;
    // Read the columns names
    if(csvFile.good()) {
        // extract the first line in the file.
        getline(csvFile, line);
        // create a string stream from line.
        stringstream stringStream(line);
        // Extract each column name and add it to columnsVector.
        while(getline(stringStream, columnName, ',')){
            this->columnsVector.push_back({columnName, vector<float> {}});
            this->featuresVector.push_back(columnName);
        }
    }
    int indexRow = 0;
    float val = 0;
    // reading the lines in the file and adding the data to the vectors of each column.
    while(getline(csvFile, line)) {
        vector<float> row = vector<float> {};
        // create a string stream of the current line.
        stringstream lineStringStream(line);
        int indexColumn = 0;
        // extract the data in this line.
        while(lineStringStream >> val){
            // add the value to the columns and rows vectors on the index.
            this->columnsVector.at(indexColumn).second.push_back(val);
            row.push_back(val);
            // if the next value is a comma, ignore it.
            if (lineStringStream.peek() == ',') {
                lineStringStream.ignore();
            }
            indexColumn++;
        }
        this->rowsVector.push_back(row);
        indexRow++;
    }
    // closing the file after reading from it.
    csvFile.close();
}

/**
 * the function return the column from the file in the index.
 * @param index - index of the column.
 * @return the column from the file in the index.
 */
vector<float> TimeSeries::getColumn(int index) const {
    return this->columnsVector.at(index).second;
}

/**
 * the function return the row from the file in the index.
 * @param index - index of the column.
 * @return the row from the file in the index.
 */
vector<float> TimeSeries::getRow(int index) const {
    return this->rowsVector.at(index);
}

/**
 * the function return the num of columns in the file.
 * @return the num of columns in the file.
 */
int TimeSeries::getNumColumns() const {
    return this->numColumns;
}

/**
 * the function return the num of rows in the file.
 * @return the num of rows in the file.
 */
int TimeSeries::getNumRows() const {
    return this->numRows;
}

/**
 * the function return the column name on index.
 * @param index - index of the column.
 * @return the column name on index.
 */
string TimeSeries::getColumnName(int index) const {
    return this->featuresVector.at(index);
}

/**
 * the function return the features vector.
 * @return the features vector.
 */
vector<string> TimeSeries::getFeaturesVector() const {
    return this->featuresVector;
}

/**
 * the method return the index that the feature appear in featuresVector.
 * @param feature - the feature to return index.
 * @return  the index that the feature appear in featuresVector.
 */
int TimeSeries::getIndexFeature(const string& feature) const {
    // moving the features vector.
    for (int i = 0; i < this->featuresVector.size(); i++) {
        // if the feature and the featuresVector on index i are equal return i.
        if (this->featuresVector.at(i) == feature) {
            return i;
        }
    }
    return -1;
}

/**
 * the function destroy the allocation memory.
 */
TimeSeries::~TimeSeries() {
    // moving on the pairs in columnsVector and freeing all memory.
    for(pair<string, vector<float>> pair : this->columnsVector) {
        pair.first.erase();
        pair.second.clear();
    }
    this->columnsVector.clear();
    // moving on the rows in rowsVector and freeing all memory.
    for (vector<float> vector : this->rowsVector) {
        vector.clear();
    }
    this->rowsVector.clear();
}
