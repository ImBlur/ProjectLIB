#ifndef REQUESTLIB
#define REQUESTLIB

#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <windows.h>

#define BUFFER_SIZE 262144
#define REQUEST_HEADER_SIZE 1024

#pragma comment(lib, "ws2_32.lib")

class Server {
    SOCKADDR_IN serverInfo = {0};
    SOCKET serverSocket = -1;
    WSADATA wsaData;

    std::vector<SOCKET> connections;

    char buffer[BUFFER_SIZE] = "";

    public:
        Server(const char* serverAddress, const int serverPort, int maxConnections = 0){
            WSAStartup(MAKEWORD(2,2), &wsaData);

            serverInfo.sin_addr.S_un.S_addr = inet_addr(serverAddress);
            serverInfo.sin_port = htons(serverPort);
            serverInfo.sin_family = AF_INET;

            if(maxConnections != 0) connections.assign(maxConnections, -1);
        }

        ~Server(){ closesocket(serverSocket); WSACleanup(); }

        void Receive(const bool whileRecv){
            SOCKET clientSocket = Connect();

            if(whileRecv) while(recv(clientSocket, buffer, BUFFER_SIZE, 0) > 0){
                printf("%s\n", buffer);
                memset(buffer, 0, BUFFER_SIZE);
            }
            else{
                recv(clientSocket, buffer, BUFFER_SIZE, 0);
                printf("%s\n", buffer);
                memset(buffer, 0, BUFFER_SIZE);
            }

            closesocket(clientSocket);
        }

        void Send(const char* message){
            if(serverSocket == SOCKET_ERROR) serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            
            SOCKET clientSocket = Connect();
            send(clientSocket, message, (int) strlen(message), 0);
        }

    private:
        SOCKET Connect(){
            if(serverSocket == SOCKET_ERROR){
                serverSocket = socket(AF_INET, SOCK_STREAM, 0);

                bind(serverSocket, (SOCKADDR *) &serverInfo, sizeof(serverInfo));
                listen(serverSocket, 0);
            }

            SOCKADDR_IN clientInfo = {0};
            int clientSize = sizeof(clientInfo);

            SOCKET clientSocket = accept(serverSocket, (SOCKADDR *) &clientInfo, &clientSize);
            return clientSocket;
        }

        void Disconnect(){
            if(serverSocket == SOCKET_ERROR) { std::cout << "socket does not exist"; return; }
            for(auto s : connections) closesocket(s);
            connections.clear();
        }

};

class Request {
    SOCKADDR_IN serverInfo = {0};
    SOCKET serverSocket = -1;
    WSADATA wsaData;
    
    protected:
        char buffer[BUFFER_SIZE] = "";
        char response[BUFFER_SIZE] = "";

    public:
        enum Mode {
            json,
            html
        };

        Request(const char* address, int port){
            WSAStartup(MAKEWORD(2,2), &wsaData);

            serverInfo.sin_family = AF_INET;
            serverInfo.sin_port = htons(port);
            serverInfo.sin_addr.S_un.S_addr = inet_addr(address);
        }

        Request(const char* address, int port, const char* message): Request(address, port){ SetBuffer(message); }

        ~Request(){ closesocket(serverSocket); WSACleanup(); }

        void SetBuffer(const char* newBuffer) { strcpy_s(buffer, BUFFER_SIZE, newBuffer); }
        char* GetBuffer() { return buffer; }

        void Connect(){
            if(serverSocket == SOCKET_ERROR) serverSocket = socket(AF_INET, SOCK_STREAM, 0);

            if(connect(serverSocket, (const SOCKADDR *) &serverInfo, sizeof(serverInfo)) == SOCKET_ERROR){
                std::cout << "connection error: " << WSAGetLastError();
                return;
            }
        }

        char* Send(){
            if(buffer == "") return nullptr;

            if(!send(serverSocket, buffer, (int) strlen(buffer), 0)){
                std::cout << "failed to send message: " << WSAGetLastError();
                return nullptr;
            }

            char temp[BUFFER_SIZE];
            while(recv(serverSocket, temp, BUFFER_SIZE, 0) > 0) strcat_s(response, BUFFER_SIZE, temp);

            return response;
        }
};

class GETRequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        GETRequest(const char* address, int port, const char* url, Mode mode): Request(address, port){
            if(mode == 0) snprintf(requestHeader, REQUEST_HEADER_SIZE, "GET %s HTTP/1.0\nHOST: %s\nContent-Type:application/json\nAccept:application/json\n\n", url, address);

            else snprintf(requestHeader, REQUEST_HEADER_SIZE, "GET %s HTTP/1.0\r\n\r\n", url);
        }

        char* Send(){
            MergeHeader();
            return Request::Send();
        }

        std::string StripData(){
            std::string resp = response;

            for(int i = 0; i < resp.size(); i++) if(resp[i] == '<' || resp[i] == '{') return resp.substr(i);

            resp.clear();
            return resp;
        }
};

class POSTRequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        POSTRequest(const char* address, int port, const char* url, Mode mode): Request(address, port){
            if(mode == 0) snprintf(requestHeader, REQUEST_HEADER_SIZE, "POST %s HTTP/1.0\nHOST: %s\nContent-Type:application/json\nAccept:application/json\n", url, address);
            
            else snprintf(requestHeader, REQUEST_HEADER_SIZE, "POST %s HTTP/1.0\r\n\r\n", url, address);
        }

        char* Send(){
            MergeHeader();
            return Request::Send();
        }

        std::string StripData(){
            std::string resp = response;

            for(int i = 0; i < resp.size(); i++) if(resp[i] == '<' || resp[i] == '{') return resp.substr(i);

            resp.clear();
            return resp;
        }
};

class PUTRequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        PUTRequest(const char* address, int port, const char* url, Mode mode=json): Request(address, port){
            snprintf(requestHeader, REQUEST_HEADER_SIZE, "PUT %s HTTP/1.0\nHOST: %s\nContent-Type:application/json\nAccept:application/json\n", url);
        }

        char* Send(){
            MergeHeader();
            return Request::Send();
        }

        std::string StripData(){
            std::string resp = response;

            for(int i = 0; i < resp.size(); i++) if(resp[i] == '<' || resp[i] == '{') return resp.substr(i);

            resp.clear();
            return resp;
        }
};

class DELETERequest: public Request {
    char requestHeader[REQUEST_HEADER_SIZE];

    void MergeHeader(){ strcat(buffer, requestHeader); }

    public:
        DELETERequest(const char* address, int port, const char* url, Mode mode=json): Request(address, port){
            snprintf(requestHeader, REQUEST_HEADER_SIZE, "DELETE %s HTTP/1.0\nHOST: %s\nContent-Type:application/json\nAccept:application/json\n", url);
        }

        char* Send(){
            MergeHeader();
            return Request::Send();
        }

        std::string StripData(){
            std::string resp = response;

            for(int i = 0; i < resp.size(); i++) if(resp[i] == '<' || resp[i] == '{') return resp.substr(i);

            resp.clear();
            return resp;
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