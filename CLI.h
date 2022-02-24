// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO* dio;
    vector<Command*> commands = vector<Command*>();

public:
    CLI(DefaultIO* dio);
    void start();
    void updateDataFromCommand(int choose);
    virtual ~CLI();
};

#endif /* CLI_H_ */