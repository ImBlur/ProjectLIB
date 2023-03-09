#include "./jsonlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <iostream>

int main(){
    std::map<std::string, std::string> map = {{"test", "\"test\""}, {"test2", "\"test2\""}};

    // std::cout << typeid(std::vector<int>).name() << std::endl;
    // std::cout << typeid(std::vector<float>).name() << std::endl;
    // std::cout << typeid(std::vector<double>).name() << std::endl;
    // std::cout << typeid(std::vector<std::string>).name() << std::endl;

    JSON<std::string> json(map);
    // json.MapToJSON(true);
    // json.JSONToMap("test.json");
    // std::unordered_map<std::string, std::string> jsonmap = json.getMap();
    // for(auto t : jsonmap) std::cout << t.first << ": " << t.second << "\n";
    json.MapToJSON("test2.json");
}