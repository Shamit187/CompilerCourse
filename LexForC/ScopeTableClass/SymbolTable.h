#pragma once
#include "SymbolInfo.h"
#include "ScopeTable.h"
#include <iostream>

class SymbolTable
{
private:
    ScopeTable* current;
    int rootScopes;
    const unsigned int bucketSize;

public:
    SymbolTable(unsigned int bucketSize);
    ~SymbolTable();

    std::string enterScope();
    std::string exitScope();

    bool insert(std::string name, std::string type);
    bool insert(std::string name, std::string type, std::string& standardOutput);
    bool remove(std::string name);
    bool remove(std::string name, std::string& standardOutput);

    SymbolInfo* lookup(std::string name);
    SymbolInfo* lookup(std::string name, std::string& returnStatement);

    std::string printCurrentScopeTable();
    std::string printAllScopeTable();

    friend std::ostream& operator<<(std::ostream& os, const SymbolTable& obj);
    std::string toString();
};