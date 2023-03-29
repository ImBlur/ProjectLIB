#ifndef REQUESTLIB
#define REQUESTLIB
#include "./jsonlib.h"

#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <windows.h>

// #define BUFFER_SIZE 4096
#define BUFFER_SIZE 262144
#define REQUEST_HEADER_SIZE 1024

#pragma comment(lib, "ws2_32.lib")

class Request {
    SOCKADDR_IN serverInfo = {0};
    
    protected:
        char buffer[BUFFER_SIZE] = "";
        char response[BUFFER_SIZE] = "";

    public:
        Request(const char* address, int port){
            serverInfo.sin_family = AF_INET;
            serverInfo.sin_port = htons(port);
            serverInfo.sin_addr.S_un.S_addr = inet_addr(address);
        }

        Request(const char* address, int port, const char* message): Request(address, port){ SetBuffer(message); }

        void SetBuffer(const char* newBuffer) { strcpy(buffer, newBuffer); }
        char* GetBuffer() { return buffer; }

        char* Send(){
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2,2), &wsaData);

            SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

            char temp[BUFFER_SIZE];

            if(connect(serverSocket, (const SOCKADDR *) &serverInfo, sizeof(serverInfo)) == SOCKET_ERROR){
                std::cout << "connection error: " << WSAGetLastError();
                return response;
            }
            
            if(!send(serverSocket, buffer, (int) strlen(buffer), 0)){
                std::cout << "failed to send message: " << WSAGetLastError();
                return response;
            }

            while(recv(serverSocket, temp, BUFFER_SIZE, 0) > 0) strcat(response, temp);

            closesocket(serverSocket);
            WSACleanup();

            return response;
        }
};

class GETRequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        GETRequest(const char* address, int port, const char* url, std::string mode="html"): Request(address, port){
            if(mode == "json") snprintf(requestHeader, REQUEST_HEADER_SIZE, "GET %s HTTP/1.0\nHOST: %s\nContent-Type:application/json\nAccept:application/json\n\n", url, address);

            else snprintf(requestHeader, REQUEST_HEADER_SIZE, "GET %s HTTP/1.0\r\n\r\n", url);
        }

        char* Send(){
            MergeHeader();
            return Request::Send();
        }
};

class POSTRequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        POSTRequest(const char* address, int port, const char* url, std::string mode="html"): Request(address, port){
            if(mode == "json") snprintf(requestHeader, REQUEST_HEADER_SIZE, "POST %s HTTP/1.0\nHOST: %s\nContent-Type:application/json\nAccept:application/json\n", url);
            
            else snprintf(requestHeader, REQUEST_HEADER_SIZE, "POST %s HTTP/1.0\r\n\r\n", url, address);
        }

        char* Send(){
            MergeHeader();
            return Request::Send();
        }
};

class PUTRequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        PUTRequest(const char* address, int port, const char* url, std::string mode="json"): Request(address, port){
            snprintf(requestHeader, REQUEST_HEADER_SIZE, "PUT %s HTTP/1.0\nHOST: %s\nContent-Type:application/json\nAccept:application/json\n", url);
        }

        char* Send(){
            MergeHeader();
            return Request::Send();
        }
};

class DELETERequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        DELETERequest(const char* address, int port, const char* url, std::string mode="json"): Request(address, port){
            snprintf(requestHeader, REQUEST_HEADER_SIZE, "DELETE %s HTTP/1.0\nHOST: %s\nContent-Type:application/json\nAccept:application/json\n", url);
        }
};

class HEADRequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        HEADRequest(const char* address, int port, const char* url): Request(address, port){
            snprintf(requestHeader, REQUEST_HEADER_SIZE, "HEAD %s HTTP/1.0\r\n\r\n", url);
        }

        char* Send(){
            MergeHeader();
            return Request::Send();
        }
};
#endif