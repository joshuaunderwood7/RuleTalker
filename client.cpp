//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <zmq.hpp>

#include "csv.h"

using namespace std;


int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    cout << "Connecting to hello world server…" << endl;
    socket.connect ("tcp://localhost:5555");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
        vector<string> data;
        data.push_back("(yo mamma)");
        data.push_back("(how fat)");

        string dataStr = makeCsv(data);
        zmq::message_t request (dataStr.size());
        memcpy (request.data (), dataStr.c_str(), dataStr.size());
        cout << "Sending data " << request_nbr << "…" << endl;
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        cout << "Received World " << request_nbr << endl;
    }

    pclose(clipsServer);
    return 0;
}
