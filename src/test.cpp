#include "./jsonlib.h"

#include <map>
#include <string>
#include <iostream>

int main(){
    std::map<std::string, std::string> map = {{"test", "\"test\""}, {"test2", "\"test2\""}};

    JSON<std::string> json(map);
    // json.MapToJSON(true);
    // json.JSONToMap("test.json");
    // std::unordered_map<std::string, std::string> jsonmap = json.getMap();
    // for(auto t : jsonmap) std::cout << t.first << ": " << t.second << "\n";
    json.MapToJSON("test2.json");
}