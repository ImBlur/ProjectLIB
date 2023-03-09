#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <stack>
#include <regex>

template <class T>
class JSON {
    private:
        std::map<std::string, T> jsonmap;

    public:
        JSON(){
            jsonmap.clear();
        }

        JSON(std::map<std::string, T> map){
            jsonmap = map;
        }

        std::map<std::string, T> getMap(){
            return jsonmap;
        }

        void MapToJSON(std::string filePath, bool cout=false){
            std::ofstream file(filePath, std::ios_base::app);

            file << "{\n";
            for(auto c : jsonmap){
                if(c.first == std::prev(jsonmap.end(), 1)->first) file << "\t\"" << c.first << "\": " << c.second;
                else file << "\t\"" << c.first << "\": " << c.second << ",\n";
            }
            file << "\n}";
        }

        void JSONToMap(std::string jsonFile){
            std::ifstream file(jsonFile);

            if(!file.is_open()){
                std::cout << "File path/name is invalid";
                return;
            }

            char c;
            std::stack<char> openStack;
            std::stack<char> closeStack;

            std::regex regexRule("\"?\\w+\"?");
            std::smatch match;

            while(file >> c){
                if(c == '{' || c == '[' || c == '(') openStack.push(c);
                if(c == '}' || c == ']' || c == ')') closeStack.push(c);

                if(c == '}' && !openStack.empty() && openStack.top() == '{' && !closeStack.empty() && closeStack.top() == '}'){ 
                    openStack.pop();
                    closeStack.pop();
                }
                if(c == ']' && !openStack.empty() && openStack.top() == '[' && !closeStack.empty() && closeStack.top() == ']'){ 
                    openStack.pop();
                    closeStack.pop();
                }
                if(c == ')' && !openStack.empty() && openStack.top() == '(' && !closeStack.empty() && closeStack.top() == ')'){ 
                    openStack.pop();
                    closeStack.pop();
                }
            }

            if(openStack.empty() && closeStack.empty()) std::cout << "Valid JSON\n";
            if(!openStack.empty() || !closeStack.empty()){
                std::cout << "Invalid JSON\n";
                return;
            }

            file.clear();
            file.seekg(0);

            std::string line;
            std::vector<std::string> matches;

            while(getline(file, line)){
                while(std::regex_search(line, match, regexRule)){
                    matches.push_back(match.str());
                    line = match.suffix().str();
                }
            }

            jsonmap.clear();
            for(int i = 0; i < matches.size() - 1; i += 2) jsonmap.insert(std::make_pair(matches[i], matches[i+1]));
            matches.clear();
        }
};

