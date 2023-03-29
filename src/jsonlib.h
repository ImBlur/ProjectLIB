#ifndef JSONLIB
#define JSONLIB

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <regex>

class JSON {
    private:
        std::vector<std::pair<std::string, std::string>> jsonmap;

        bool CheckJSON(std::string filePath){
            std::ifstream file(filePath);
            if(!file.is_open()){
                std::cout << "File name/path is invalid";
                return false;
            }

            std::stack<char> openStack;
            std::stack<char> closeStack;

            char c;
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

            if(openStack.empty() && closeStack.empty()) return true;
            if(!openStack.empty() || !closeStack.empty()) return false;
            return false;
        }

        int GetIndex(int startIndex, std::vector<std::string> v){
            for(int i = startIndex + 1; i < v.size(); i++){
                if(*(std::prev(v[i].end()).base()) == '\n') return i;
            }
            return v.size() - 1;
        }
        
        void RemoveSpecialCharacters(std::vector<std::string> &v){
            for(int i = v.size() - 1; i >= 0; i--){
                bool good = false;
                for(auto l : v[i]) if((l >= 'A' && l <= 'Z') || (l >= 'a' && l <= 'z') || (l >= '0' && l <= '9')) good = true;
                if(!good) v.erase(v.begin() + i);
            }
        }

        std::pair<std::string, std::string> CreatePair(int startIndex, int endIndex, std::vector<std::string> &v){
            std::vector<int> indexToDelete = {startIndex};
            std::pair<std::string, std::string> tempPair = std::pair(v[startIndex], "");

            for(int i = startIndex + 1; i <= endIndex; i++){
                if(i == endIndex) tempPair.second += v[i];
                else tempPair.second += v[i] + " ";
                indexToDelete.push_back(i);
            }

            for(int i = indexToDelete.size() - 1; i >= 0; i--) v.erase(v.begin() + indexToDelete[i]);
            return tempPair;
        }

    public:
        JSON(){
            jsonmap.clear();
        }

        JSON(std::vector<std::pair<std::string, std::string>> map){
            jsonmap = map;
        }

        std::vector<std::pair<std::string, std::string>> getMap(){
            return jsonmap;
        }

        void StringToJSON(std::string jsonString){
            std::regex regexRule("[[:graph:]]+\\n?");
            std::smatch match;

            std::vector<std::string> matches;

            while(std::regex_search(jsonString, match, regexRule)){
                matches.push_back(match.str());
                jsonString = match.suffix().str();
            }

            matches.erase(matches.begin());
            matches.erase(std::prev(matches.end(), 1));

            jsonmap.clear();
            for(int i = 0; i < matches.size(); i++){
                if(*std::prev(matches[i].end(), 1).base() != ':') continue;

                // std::cout << i << " " << GetIndex(i, matches) << "\n";
                std::pair<std::string, std::string> pair = CreatePair(i, GetIndex(i, matches), matches);
                pair.second.erase(std::prev(pair.second.end()));
                // std::cout << "pair: " << pair.first << " " << pair.second << "\n";
                jsonmap.push_back(pair);
                i--;
            }
            matches.clear();
        }

        std::string JSONToString(){
            if(jsonmap.empty()) return "";
            std::string temp = "{ ";
            for(auto pair : jsonmap){
                temp += pair.first + " " + pair.second + "\n";
            }
            temp += " }";
            return temp;
        }

        void MapToJSONFile(std::string filePath){
            if(jsonmap.empty()) return;
            std::ofstream file(filePath, std::ios_base::app);

            file << "{\n";
            for(auto c : jsonmap){
                if(c.first == std::prev(jsonmap.end(), 1)->first) file << "\t" << c.first << " " << c.second;
                else file << "\t" << c.first << " " << c.second << "\n";
            }
            file << "\n}";
        }

        void JSONFileToMap(std::string jsonFile){
            std::ifstream file(jsonFile);

            if(!file.is_open()){
                std::cout << "File path/name is invalid";
                return;
            }

            if(!CheckJSON(jsonFile)) return;

            std::regex regexRule("[[:graph:]]+\\n?");
            std::smatch match;

            std::string line;
            std::string fileContent = "";
            std::vector<std::string> matches;

            while(getline(file, line)){
                fileContent += line + "\n";
            }

            // std::cout << fileContent;

            while(std::regex_search(fileContent, match, regexRule)){
                matches.push_back(match.str());
                fileContent = match.suffix().str();
            }
            
            matches.erase(matches.begin());
            matches.erase(std::prev(matches.end(), 1));
            // for(int i = 0; i < matches.size(); i++){
            //     std::cout << "match " << i + 1 << ": ";
            //     for(auto s : matches[i]){
            //         if(s == '\n') std::cout << "\\n";
            //         else std::cout << s;
            //         // std::cout << s;
            //     }
            //     std::cout << "\n";
            // }

            jsonmap.clear();
            for(int i = 0; i < matches.size(); i++){
                if(*std::prev(matches[i].end(), 1).base() != ':') continue;

                // std::cout << i << " " << GetIndex(i, matches) << "\n";
                std::pair<std::string, std::string> pair = CreatePair(i, GetIndex(i, matches), matches);
                pair.second.erase(std::prev(pair.second.end()));
                // std::cout << "pair: " << pair.first << " " << pair.second << "\n";
                jsonmap.push_back(pair);
                i--;
            }
            matches.clear();
        }
};
#endif