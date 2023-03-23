#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <regex>

class JSON {
    private:
        std::map<std::string, std::string> jsonmap;

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

        JSON(std::map<std::string, std::string> map){
            jsonmap = map;
        }

        std::map<std::string, std::string> getMap(){
            return jsonmap;
        }

        void MapToJSON(std::string filePath, bool cout=false){
            std::ofstream file(filePath, std::ios_base::app);

            file << "{\n";
            for(auto c : jsonmap){
                if(c.first == std::prev(jsonmap.end(), 1)->first) file << "\t" << c.first << " " << c.second;
                else file << "\t" << c.first << " " << c.second << "\n";
            }
            file << "\n}";
        }

        void JSONToMap(std::string jsonFile){
            std::ifstream file(jsonFile);

            if(!file.is_open()){
                std::cout << "File path/name is invalid";
                return;
            }

            if(!CheckJSON(jsonFile)) return;

            std::regex regexRule("[[:graph:]]+\\n?");
            std::smatch match;

            std::string line;
            std::vector<std::string> matches;

            while(getline(file, line)){
                while(std::regex_search(line, match, regexRule)){
                    matches.push_back(match.str());
                    line = match.suffix().str();
                }
            }
            
            matches.erase(matches.begin());
            matches.erase(std::prev(matches.end(), 1));
            // for(int i = 0; i < matches.size(); i++) std::cout << "match " << i + 1 << ": " << matches[i] << "\n";

            jsonmap.clear();
            for(int i = 0; i < matches.size(); i++){
                if(*std::prev(matches[i].end(), 1).base() == ':'){
                    std::pair<std::string, std::string> pair = CreatePair(i, GetIndex(i, matches), matches);
                    jsonmap.insert(pair);
                }
            }
            matches.clear();
        }
};