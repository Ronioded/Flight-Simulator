// Roni Oded, ID: 318798782 & Eden Cohen, ID:318758778.
#include "Server.h"

/**
 * Constructor.
 * @param socketID of the client.
 */
SocketIO::SocketIO(int socketID) {
    this->socketID = socketID;
}

/**
 * read data from the client.
 * @return the string from the client.
 */
string SocketIO::read(){
    string s;
    char receivedChar = 0;

    // read the first char.
    ::read(this->socketID,&receivedChar,sizeof(char));

    // read chars until the end of line.
    while(receivedChar!='\n'){

        // concat the chars.
        s+=receivedChar;

        // read another char
        ::read(this->socketID,&receivedChar,sizeof(char));
    }
    // return the line.
    return s;
}

/**
 * send the given text to the client.
 * @param text to sent.
 */
void SocketIO::write(string text){
    send(this->socketID, text.c_str(), text.length(),0);
}

/**
 * send float number to the client.
 * @param f - float number to send.
 */
void SocketIO::write(float f) {
    // convert the number to string and cut redundant zeros- call the function that send string.
    stringstream stream;
    stream<<f;
    write(stream.str());
}

/**
 * read float from client.
 * @param f he pointer to float where to save the float from the client.
 */
void SocketIO::read(float* f) {
    ssize_t n = ::read(this->socketID,f,255);
}

/**
 * destructor.
 */
SocketIO::~SocketIO(){}

/**
 * the function handle the client, start the cli.
 */
void AnomalyDetectionHandler::handle(int clientID){
    // creat socketIO from the clientID, create new CLI with socketIO and start the cli.
    SocketIO socketIO = SocketIO(clientID);
    CLI cli = CLI(&socketIO);
    cli.start();
}

/**
 * Constructor.
 * @param port - that the server listen to.
 */
Server::Server(int port)throw (const char*) {
    // initialize the pointer to the thread.
    this->t = nullptr;
    // open socket for the server.
    this->serverID = socket(AF_INET, SOCK_STREAM, 0);
    // check the socket opening if failed.
    if (this->serverID < 0) {
        throw "create socket failure";
    }

    // define the server's socket.
    struct sockaddr_in sock_ad{};
    sock_ad.sin_family = AF_INET;
    sock_ad.sin_addr.s_addr = INADDR_ANY;
    sock_ad.sin_port = htons(port);
    socklen_t len = sizeof(sock_ad);
    int b = bind(this->serverID,(struct sockaddr*) &sock_ad, len);
    // check if the bind failed.
    if (b < 0) {
        throw "bind socket failure";
    }
    int l = listen(this->serverID, 5);
    // check if the listening failed.
    if (l < 0) {
        throw "listen socket failure";
    }
}
/**
 * open thread to the server.
 * @param ch client handler- the function that runs on the thread need the client handler as argument.
 */
void Server::start(ClientHandler& ch)throw(const char*){
    // open start with new thread.
    this->t = new thread (&Server::startThread, this, ref(ch));
}

/**
 * new threaded function which accept clients.
 * @param ch - the client handler.
 */
void Server::startThread(ClientHandler& ch)throw(const char*){
    // run the thread until the server notifies to stop.
    while (!this->toStop) {
        struct sockaddr_in clientSocket{};
        socklen_t socklen = sizeof(clientSocket);
        // wait for client to connect.
        alarm(5);
        int clientID = accept(this->serverID, (struct sockaddr *) &clientSocket, &socklen);
        // check if accept failed.
        if (clientID < 0) {
            throw "client socket failure";
        }
        // call the handle function that runs the CLI.
        ch.handle(clientID);
        // close the client socket when the handling finish.
        close(clientID);
    }
    // close the server socket.
    close(this->serverID);
}

/**
 * stop waiting for clients.
 */
void Server::stop(){
    // notify to stop accept new clients.
    this->toStop = true;
    // terminate the thread.
    this->t->join(); // do not delete this!
}

/**
 * destructor.
 */
Server::~Server() {}