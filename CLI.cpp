// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#include "CLI.h"

/**
 * constructor
 * @param dio - the default io.
 */
CLI::CLI(DefaultIO* dio) {
    // initializing the fields.
    this->dio = dio;
    this->commands.push_back(new CsvCommand(dio));
    this->commands.push_back(new AlgorithmSettingCommand(dio));
    this->commands.push_back(new DetectAnomaliesCommand(dio));
    this->commands.push_back(new DisplayResultsCommand(dio));
    this->commands.push_back(new UploadAnomaliesCommand(dio));
    this->commands.push_back(new ExitCommand(dio));
}

/**
 * the method start the menu until exit from the program.
 */
void CLI::start(){
    while (true) {
        // displaying the menu.
        string choose;
        this->dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        int size = this->commands.size();
        // passing the commands and write the description.
        for (int i = 0; i < size; i++) {
            this->dio->write(to_string(i + 1));
            this->dio->write(".");
            this->dio->write(this->commands[i]->getDescription());
            this->dio->write("\n");
        }

        // get choose from the client.
        choose = this->dio->read();
        // execute the command in index choose - 1. (cause the commands start from 0).
        this->commands[stoi(choose) - 1]->execute();
        // if the chose is 6, return.
        if (stoi(choose) == 6) {
            return;
        }
        // update other commands with the data.
        CLI::updateDataFromCommand(stoi(choose));
    }
}

/**
 * the method update data to all commands.
 * @param choose - the choose from client.
 */
void CLI::updateDataFromCommand(int choose){
    // if choose is 3, update the reports and time series.
    if (choose == 3) {
        // passing the commands and update reports and time series.
        for (int i = 0; i < this->commands.size(); i++) {
            // not update to i = 2.
            if (i != 2) {
                // passing the anomaly report of reports in i=2 and push to the reports of other i.
                for (const AnomalyReport &ar: this->commands[2]->reports) {
                    this->commands[i]->reports.push_back(ar);
                }
                this->commands[i]->timeSeries =  this->commands[2]->timeSeries;
            }
        }
    }

    // if choose is 2, update anomalyDetector.
    if (choose == 2) {
        // passing the commands and update anomaly detector.
        for (Command* c: this->commands) {
            c->anomalyDetector =  this->commands[choose - 1]->anomalyDetector;
        }
    }
}

/**
 * destructor.
 */
CLI::~CLI() {
    // passing commends and delete them.
    for(Command* c: this->commands) {
        delete c;
    }
    // clear commands.
    this->commands.clear();
}