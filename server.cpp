#include <clips.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <zmq.hpp>

#include "csv.h"

//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//

using namespace std;


int main(int argc, const char *argv[])
{
    cout << "Prepare our context and socket" << endl;
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");

    vector<string> facts;

    //Set up the CLIPS environment and variables
    void*       theEnv = CreateEnvironment();
    DATA_OBJECT rtn;
    EnvLoad(theEnv, "rules.clp");

    cout << "Server runnning" << endl;
    while (true) {
        zmq::message_t request;

        cout << "Wait for next request from client" << endl;
        socket.recv (&request);
        std::cout << "Received " << (char *)request.data() << std::endl;
        facts = splitCsv((char *)request.data());

        cout << "Do some work with clips" << endl;
        EnvReset(theEnv);

        //EnvEval(theEnv, "(watch all)", &rtn);
        for (auto fact : facts)
            EnvAssertString(theEnv, fact.c_str());

        EnvRun(theEnv,-1);
        //EnvEval(theEnv, "(facts)", &rtn);

        cout << "Send reply back to client" << endl;
        zmq::message_t reply (5);
        memcpy (reply.data (), "World", 5);
        socket.send (reply);
    }
    
    cout << "Server done" << endl;
    return 0;
}

