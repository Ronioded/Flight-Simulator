// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

/**
 * class DefaultIO, responsible on read and write.
 */
class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO()=default;

    /**
     * the method write the data in the CSVfileName.
     * @param CSVfileName - name of the file to write to.
     * @param data - the data to write.
     */
    virtual void writeFile(const char* CSVfileName, const string& data){
        // Create csv file with the given name and the given data.
        fstream fout;
        // opens an existing csv file or creates a new file and write data to the file.
        fout.open(CSVfileName, fstream::app);
        fout << data;
        fout << "\n";
        fout.close();
    }
};

/**
 * methos command- responsible on the different commands.
 */
class Command{
    DefaultIO* dio;

public:
    HybridAnomalyDetector anomalyDetector = HybridAnomalyDetector();
    string description;
    TimeSeries timeSeries;
    vector<AnomalyReport> reports = vector<AnomalyReport>();

    /**
     * constructor.
     * @param dio - the default IO.
     */
    Command(DefaultIO* dio):dio(dio){}

    /**
     * execute the command.
     */
    virtual void execute()=0;

    /**
     * destructor.
     */
    virtual ~Command(){
        this->reports.clear();
    }

    /**
     * the method return the description of the command.
     * @return the description.
     */
    string getDescription() const {
        return this->description;
    }

    /**
     * the method return the default IO.
     * @return the default IO.
     */
    DefaultIO* getIO() const {
        return this->dio;
    }
};


/**
 * Class that represent 1st command- create csv files from given data from the client.
 */
class CsvCommand : public Command {

public:

    /**
     * Constructor.
     * @param dio - default io.
     */
    CsvCommand(DefaultIO* dio): Command(dio){
        this->description = "upload a time series csv file";
    }

    /**
     * Execute the command.
     */
    void execute() override {

        // Tell the client to upload the csv file data.
        this->getIO()->write("Please upload your local train CSV file.\n");

        // read the data line by line until we gote "done" from the client.
        string trainData;
        do {
            // get line from client.
            trainData = this->getIO()->read();
            // if the reading didn't end write to the file.
            if (trainData != "done") {
                getIO()->writeFile("anomalyTrain.csv", trainData);
            }
        } while (trainData != "done");

        // tell the client the upload completed.
        this->getIO()->write("Upload complete.\n");

        // ask for the second file - for the test csv.
        this->getIO()->write("Please upload your local test CSV file.\n");

        // read the data line by line until we gote "done" from the client.
        string testData;
        do {
            // get line from client.
            testData = this->getIO()->read();
            // if the reading didn't end write to the file.
            if (testData != "done") {
                getIO()->writeFile("anomalyTest.csv", testData);
            }
        } while (testData != "done");

        // tell the client the upload completed.
        this->getIO()->write("Upload complete.\n");
    }
};

/**
 * Class that represent 2nd command- let the client choose a new threshold.
 */
class AlgorithmSettingCommand : public Command {
public:

    /**
     * Constructor.
     * @param dio - default io.
     */
    AlgorithmSettingCommand(DefaultIO* dio): Command(dio){
        this->description = "algorithm settings";
    }

    /**
     * Execute the command.
     */
    void execute() override {
        // print the current threshold:
        this->getIO()->write("The current correlation threshold is ");
        this->getIO()->write(this->anomalyDetector.threshold);
        this->getIO()->write("\n");

        // Tell the client to insert new threshold.
        this->getIO()->write("Type a new threshold\n");

        // get new threshold - until we got valid value from the client.
        float newThreshold = NAN;
        do {
            this->getIO()->read(&newThreshold);

            // if the threshold invalid print a message and ask for new one until the input is valid.
            if ((newThreshold < 0) || (newThreshold > 1)) {
                this->getIO()->write("please choose a value between 0 and 1\n");
            }
        } while ((newThreshold < 0) || (newThreshold > 1));

        // update the new threshold.
        this->anomalyDetector.threshold = newThreshold;
    }
};

/**
 * Class that represent 3rd command - run the hybrid algorithm on the csv file we created on the first command.
 */
class DetectAnomaliesCommand : public Command {

public:

    /**
     * Constructor.
     * @param dio - default io.
     */
    DetectAnomaliesCommand(DefaultIO* dio): Command(dio){
        this->description = "detect anomalies";
    }

    /**
    * Execute the command.
    */
    void execute() override {

        // the names of the file to detect from.
        const char* csvTest= "anomalyTest.csv";
        const char* csvTrain = "anomalyTrain.csv";

        // Create time series from the csv files.
        TimeSeries tsTest =  TimeSeries(csvTest);
        TimeSeries tsTrain = TimeSeries(csvTrain);

        // initialize the timeSeries field.
        this->timeSeries = tsTest;

        // detect.
        this->anomalyDetector.learnNormal(tsTrain);
        this->reports = this->anomalyDetector.detect(tsTest);

        // tell the client the detection completed.
        this->getIO()->write("anomaly detection complete.\n");
    }
};

/**
 * the class display the results of the server. implements Command.
 */
class DisplayResultsCommand: public Command{

public:
    /**
     * constructor.
     * @param dio - the defaultIO.
     */
    DisplayResultsCommand(DefaultIO* dio): Command(dio){
        this->description = "display results";
    }

    /**
     * the method execute the command.
     */
    void execute() override {
        // passing the reports and print the time steps and descriptions.
        for (const AnomalyReport& ar : this->reports) {
            this->getIO()->write(to_string(ar.timeStep));
            this->getIO()->write("\t");
            this->getIO()->write(ar.description);
            this->getIO()->write("\n");
        }
        this->getIO()->write("Done.\n");
    }

    /**
     * destructor.
     */
    virtual ~DisplayResultsCommand()=default;
};

/**
 * the class upload anomalies. implements Command.
 */
class UploadAnomaliesCommand: public Command{
public:

    /**
     * constructor.
     * @param dio - the default IO.
     */
    UploadAnomaliesCommand(DefaultIO *dio): Command(dio) {
        this->description = "upload anomalies and analyze results";
    }

    /**
     * the method execute the command.
     */
    void execute() override {
        // read the file from the client.
        vector<pair<int,int>> exceptions = readFile();
        // union reports with same description.
        vector<pair<int,int>> unionReports = unionReportsVector(this->reports);

        // initialize P,N.
        float P = exceptions.size();
        auto N = float(this->timeSeries.getNumRows());

        // passing the exception and update N.
        for (pair<int,int> p : exceptions) {
            float rows = float(p.second) - float(p.first) + 1;
            N -= rows;
        }

        float TP = 0;
        float FP = 0;

        // update FP,TP.
        for (pair<int,int> report : unionReports) {
            bool flag = false;
            for(pair<int,int> exception: exceptions) {
                // if there is intersection of the report and exception, turning flag to true.
                if (!((report.second < exception.first) || (exception.second < report.first))) {
                    flag = true;
                }
            }
            // if the flag became true, there is intersection the report and some exception so increment TP.
            if (flag) {
                TP++;
            // else, there were no intersection of the report and all exceptions so increment FP.
            } else {
                FP++;
            }
        }
        // round TPRate,FPRate according to 3 numbers after the point.
        float roundTPRate = roundThreeNum(TP / P, 3);
        float roundFPRate = roundThreeNum(FP / N, 3);

        // display TPRate,FPRate.
        this->getIO()->write("True Positive Rate: ");
        this->getIO()->write(roundTPRate);
        this->getIO()->write("\n");
        this->getIO()->write("False Positive Rate: ");
        this->getIO()->write(roundFPRate);
        this->getIO()->write("\n");
    }

    /**
     * the method round a number up to precision numbers after the point.
     * @param value - the value to round.
     * @param precision - the num of numbers after the point.
     * @return the value after the round
     */
    static float roundThreeNum(float value, unsigned char prec) {
        auto x= (float)((int)(value*1000)/(float)1000);
        float pow10 = pow(10.0f, (float)prec);
        return round(x*pow10)/pow10;
    }

    /**
     * the method union reports with same description.
     * @param reports - the vector to union.
     * @return a new vector with pairs of the union reports.
     */
    static vector<pair<int,int>> unionReportsVector(const vector<AnomalyReport>& reports) {
        vector<pair<int,int>> unionReports = vector<pair<int,int>>();
        int size = reports.size();
        // passing all reports.
        for (int i = 0; i < size; i++) {
            pair<int,int> p;
            // initialize a pair which the first element is the time step of reports[i].
            p.first = reports[i].timeStep;
            // while we didn't reach to the last element and the descriptions are equal, increment i.
            while((i != size - 1) && (reports[i+1].description == reports[i].description)) {
                i++;
            }
            // the second element is the time step of reports[i] after incrementing i.
            p.second = reports[i].timeStep;
            unionReports.push_back(p);
        }
        return unionReports;
    }

    /**
     * the method read the file from the client.
     * @return
     */
    vector<pair<int,int>> readFile() {
        // ask for local anomalies file.
        this->getIO()->write("Please upload your local anomalies file.\n");

        // initialize line, val and vector of exceptions.
        string line;
        int val = 0;
        vector<pair<int,int>> exceptions = vector<pair<int,int>>();

        // reading lines while there is no 'done';
        do {
            // read line and create a string stream from line.
            line = this->getIO()->read();
            stringstream lineStringStream(line);
            pair<int,int> p;
            int i = 0;
            // if line is different than done, update the pair and add to the vector.
            if (line != "done") {
                // extract the data in this line and update the pair.
                while (lineStringStream >> val) {
                    if (i == 0) {
                        p.first = val;
                    } else {
                        p.second = val;
                    }
                    if (lineStringStream.peek() == ',') {
                        lineStringStream.ignore();
                    }
                    i++;
                }
                exceptions.push_back(p);
            }
        } while (line!="done");
        this->getIO()->write("Upload complete.\n");
        return exceptions;
    }

    virtual ~UploadAnomaliesCommand()=default;
};

/**
 * class ExitCommand, implementd Command, exit from the program.
 */
class ExitCommand: public Command{

public:
    /**
     * constructor.
     * @param dio - the default IO.
     */
    ExitCommand(DefaultIO* dio):Command(dio){
        this->description = "exit";
    }

    /**
     * exit from the program.
     */
    void execute() override { }

    /**
     * destructor.
     */
    virtual ~ExitCommand()=default;
};

#endif /* COMMANDS_H_ */