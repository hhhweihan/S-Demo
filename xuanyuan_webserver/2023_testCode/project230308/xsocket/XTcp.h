#pragma once

#ifdef XSOCKET_EXPORTS
#define XSOCKET_API_declspec(dllexport)
#else
#define XSOCKET_API_declspec(dllimport)
#endif


#include<string>
class XSOCKET_API XTcp
{ 
public:
    int CreateSocket();
    bool Bind(unsigned short port);
    XTcp Accept();
    void Close();
    int Recv(char *buf, int bufsize);
    int Send(const char* buf, int sendsize);
    XTcp();
    virtual ~XTcp();
    int sock = 0;
    unsigned short port = 0;
    std::string ip;
};

