#pragma once
#include <string>
#include <iostream>

class SymbolInfo
{
private:
    std::string name;
    std::string type;
    SymbolInfo *next;

public:
    SymbolInfo(std::string name, std::string type);
    ~SymbolInfo();

    //io operation
    std::string getName();
    std::string getType();
    SymbolInfo* getNext();

    void setName(std::string newName);
    void setType(std::string newType);
    void setNext(SymbolInfo* newNext);

    //util function
    std::string toString();
    friend std::ostream& operator<<(std::ostream& os, const SymbolInfo& obj);
};