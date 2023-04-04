# Project LIB

This is a project made for personal use and will most likely never be public.

## Instalation

To install and use these libraries you need to just simply donwload them, put them in a folder inside your project folder and then inside the `.h` or `.cpp` file make sure to `#include "path/to/library.h"`

## Documentation

### JSONLib

To declare a JSON type variable: `JSON variableName;`

To use the functions:

- First declare the variable using `JSON variableName;`
- Call the function you want to use using `variableName.function(params);`
  - Params are set depending on the function. Some function do not require params.

Functions:

- `void StringToJSON(std::string str)` - converts a std::string into a json object.
- `std::string JSONToString(void)` - converts the json object into a std::string.
- `void MapToJSONFile(std::string filePath)` - converts the json object into a `.json` file.
- `void JSONFileToMap(std::string filePath)` - converts the `.json` file into a json object.
- `std::vector<std::pair<std::string, std::string>> GetMap(void)` - returns the json object into a vector of pairs of std::string

### RequestLib

To declare a request variable: `typeOfRequest requestName(requestParams);`

Types of requests with the required params:

- `Request(const char* address, int port, const char* sendBuffer)`
  - sendBuffer = the buffer (message) you want to send
- `GETRequest(const char* address, int port, const char* url, const char* mode="html" || "json")`
- `POSTRequest(const char* address, int port, const char* url, const char* mode="html" || "json")`
- `PUTRequest(const char* address, int port, const char* url, const char* mode="json")`
- `DELETERequest(const char* address, int port, const char* url, const char* mode="json")`
- `HEADRequest(const char* address, int port, const char* url)`

To call the functions:

- Declare a request
- Call a function using: `requestName.function();`

Functions:

- `char* Send(void)` - sends the request and returns the response
- `std::string StripData()` - strips the request header from the data and returns the data (json or html)
  - !!! `HEADRequest` does not have this function !!!
