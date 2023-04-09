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

### Timer Lib

Adds the ability to monitor code execution time using a class `Timer` that is inside the `utils` namespace. It will print the time it took to executed the code in microseconds and miliseconds

Examples:

- Monitor the entire code:

```cpp
#include "timer.h"

int main(){
  utils::Timer t;
  // your code goes here
}
```

- Monitor a specific part of the code:

```cpp
#include "timer.h"

int main(){
  //some code here

  {
    utils::Timer t;
    //code you want to time here
  }

}
```

### TextColor Lib

Implements a nice and easy way to work with colored text. Everything added by this library can be found in `utils` namespace

Example:

```cpp
#include "textcolor.h"

#include <iostream>

int main(){
  utils::TextColor color(utils::color::FG_RED); // creates a red font color
  utils::TextColor def(utils::color::FG_DEFAULT); // resets the font color to the default one

  // FG = foreground (font color)
  // BG = background (background color)

  std::cout << "This " << color << "word" << def << "is red"; // prints "This *word* (with red font) is red"
  
  // Everything after `color` will have the font color red. Everything after `def` will be the default color
}
```

### Prints Lib

Implements functions to handle errors and warnings. Everything inside this library can be found in the `utils` namespace.

Example:

```cpp
#include "prints.h"

#include <vector>

int main(){
  std::vector<int> v;

  if(v.empty()){
    utils::error("The vector is empty"); // prints an error message that the vector is empty
    return 0;
  }

  utils::warning("The vector contains at least one element"); // prints a warning that the vector contains at least one element
}
```
