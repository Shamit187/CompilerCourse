#include "SymbolInfo.h"

SymbolInfo::SymbolInfo(std::string name,
                       std::string type)
        :name{name},
         type{type},
         next{nullptr}
{}

SymbolInfo::~SymbolInfo()
{}

std::string SymbolInfo::getName(){return name;}

std::string SymbolInfo::getType(){return type;}

SymbolInfo* SymbolInfo::getNext(){return next;}

void SymbolInfo::setName(std::string newName){this->name = newName;}

void SymbolInfo::setType(std::string newType){this->type = newType;}

void SymbolInfo::setNext(SymbolInfo* newNext){this->next = newNext;}

std::ostream& operator<<(std::ostream& os, const SymbolInfo& obj){
    os << "< " << obj.name << " : " << obj.type << " >";
    return os;
}

std::string SymbolInfo::toString() {
    return std::string( "< " + name + " : " + type + " >");
}
