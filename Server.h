// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.
#ifndef SERVER_H_
#define SERVER_H_

#include "commands.h"
#include "CLI.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include <string>

using namespace std;

/**
 * SocketIO extends DefaultIO, send and accept data via sockets.
 */
class SocketIO:public DefaultIO {
    int socketID;

public:
    SocketIO(int socketID);
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f) ;
    ~SocketIO();
};

/**
 * in charge of handling with the clients.
 */
class ClientHandler {
public:
    virtual void handle(int clientID)=0;
};

/**
 * in charge of handling with the clients that wants to detect.
 */
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID);
};

/**
 * In charge of getting information from the client and runs the Anomaly detection by th given client handler.
 */
class Server {
    thread* t;
    bool toStop = false;
    int serverID;

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void startThread(ClientHandler& ch) throw(const char*);
    void stop();
};

#endif /* SERVER_H_ */