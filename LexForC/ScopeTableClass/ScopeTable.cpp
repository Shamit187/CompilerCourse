#include "ScopeTable.h"

ScopeTable::ScopeTable(unsigned int size, ScopeTable* parent, std::string id)
        : size{size},
          numberOfChild{0},
          parent{parent},
          hashTable{new SymbolInfo*[size]},
          id{id}
{
    for(int i = 0; i < size; i++)
        hashTable[i] = nullptr;
}

ScopeTable::~ScopeTable()
{
    SymbolInfo* currentChain;
    SymbolInfo* next;
    for(int i = 0; i < size; i++){
        currentChain = hashTable[i];
        while(currentChain!= nullptr){
            next = currentChain->getNext();
            delete currentChain;
            currentChain = next;
        }
    }
    delete[] this->hashTable;
}

bool ScopeTable::insert(std::string name, std::string type)
{
    SymbolInfo* symbolInfo = new SymbolInfo(name, type);
    unsigned int bucket = sdbmHash(name, size);

    SymbolInfo* current = hashTable[bucket];
    SymbolInfo* prev = nullptr;

    //for standard output
    int i = bucket;
    int j = 0;

    if(current == nullptr)
        /*
            nothing in the bucket,
            no chance of similarity,
            insert and return
        */
    {
        hashTable[bucket] = symbolInfo;
        return true;
    }
    else
    {
        while(current != nullptr){
            j++;
            prev = current;
            current = prev->getNext();
            /*
                contains in the chain
                return false
            */
            if(prev->getName() == name)
            {
                delete symbolInfo;
                return false;
            }
        }
        /*
            couldn't find in chain
            add after the last element
        */
        prev->setNext(symbolInfo);
    }
    return true;
}

bool ScopeTable::insert(std::string name, std::string type, std::string &standardOutput) {
    SymbolInfo* symbolInfo = new SymbolInfo(name, type);
    unsigned int bucket = sdbmHash(name, size);

    SymbolInfo* current = hashTable[bucket];
    SymbolInfo* prev = nullptr;

    //for standard output
    int i = bucket;
    int j = 0;

    if(current == nullptr)
        /*
            nothing in the bucket,
            no chance of similarity,
            insert and return
        */
    {
        hashTable[bucket] = symbolInfo;
        standardOutput +=
                "Inserted in ScopeTable# "
                + id + " at position "
                + std::to_string(i) + ", "
                + std::to_string(j);
        return true;
    }
    else
    {
        while(current != nullptr){
            j++;
            prev = current;
            current = prev->getNext();
            /*
                contains in the chain
                return false
            */
            if(prev->getName() == name)
            {
                delete symbolInfo;
                standardOutput +=
                        + "<" + name + "," + type + ">"
                        + " already exists in current ScopeTable";
                return false;
            }
        }
        /*
            couldn't find in chain
            add after the last element
        */
        prev->setNext(symbolInfo);
        standardOutput +=
                "Inserted in ScopeTable# "
                + id + " at position "
                + std::to_string(i) + ", "
                + std::to_string(j);
    }
    return true;
}

bool ScopeTable::remove(SymbolInfo* symbolInfo)
{
    if(symbolInfo == nullptr) return false;

    unsigned int bucket = sdbmHash(symbolInfo->getName(), size);

    SymbolInfo* current = hashTable[bucket];
    SymbolInfo* prev = nullptr;

    if(current == nullptr)
        return false;
    else if(current == symbolInfo)
    {
        hashTable[bucket] = current->getNext();
        delete current;
    }
    else{
        while(current != nullptr)
        {
            if(current == symbolInfo)
            {
                prev->setNext(current->getNext());
                delete current;
                return true;
            }
            prev = current;
            current = prev->getNext();
        }
    }
    return false;
}

bool ScopeTable::remove(SymbolInfo *symbolInfo, std::string &standardOutput) {
    if(symbolInfo == nullptr)
        return false;

    unsigned int bucket = sdbmHash(symbolInfo->getName(), size);

    SymbolInfo* current = hashTable[bucket];
    SymbolInfo* prev = nullptr;

    if(current == nullptr)
        return false;
    else if(current == symbolInfo)
    {
        hashTable[bucket] = current->getNext();
        standardOutput.append("\nDeleted Entry " + std::to_string(bucket) + ",0 from current ScopeTable");
        delete current;
        return true;
    }
    else{
        int j = 0;
        while(current != nullptr)
        {
            if(current == symbolInfo)
            {
                prev->setNext(current->getNext());
                delete current;
                standardOutput.append("\n Deleted Entry " + std::to_string(bucket) + "," + std::to_string(j) + "from current ScopeTable");
                return true;
            }
            prev = current;
            current = prev->getNext();
            j++;
        }
    }
    return false;
}

SymbolInfo* ScopeTable::lookup(std::string name){

    unsigned int bucket = sdbmHash(name, size);

    SymbolInfo* current = hashTable[bucket];
    SymbolInfo* prev = nullptr;

    if(current == nullptr)
        return nullptr;
    else if(current->getName() == name)
        return current;
    else{
        while(current != nullptr)
        {
            if(current->getName() == name)
                return current;
            prev = current;
            current = prev->getNext();
        }
        return nullptr;
    }
}

SymbolInfo *ScopeTable::lookup(std::string name, std::string &standardOutput) {
    unsigned int bucket = sdbmHash(name, size);
    int i = bucket, j = 0;

    SymbolInfo* current = hashTable[bucket];
    SymbolInfo* prev = nullptr;

    if(current == nullptr)
    {
        standardOutput = "Not Found";
        return nullptr;
    }
    else if(current->getName() == name)
    {
        standardOutput.append("Found in ScopeTable# "
                              + id + " at position "
                              + std::to_string(i) + ", "
                              + std::to_string(j));
        return current;
    }
    else{
        while(current != nullptr)
        {
            if(current->getName() == name)
            {
                standardOutput.append("Found in ScopeTable# "
                                      + id + " at position "
                                      + std::to_string(i) + ", "
                                      + std::to_string(j));
                return current;
            }
            j++;
            prev = current;
            current = prev->getNext();
        }
        standardOutput = "Not Found";
        return nullptr;
    }
}

std::string ScopeTable::getScopeId() const{
    return id;
}

void ScopeTable::print() const{
    std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& os, const ScopeTable& obj){
    os << "ScopeTable # " << obj.id << std::endl;

    for(int i = 0; i < obj.size; i++){
        os << i << " --> ";
        SymbolInfo* current = obj.hashTable[i];
        while(current != nullptr){
            os << *current << "  ";
            current = current->getNext();
        }
        if(i != obj.size - 1)
            os << std::endl;
    }

    return os;
}

std::string ScopeTable::toString() {
    std::string returnStatement = "";
    returnStatement += "ScopeTable # " + id + "\n";

    for(int i = 0; i < size; i++){
        if(hashTable[i]!= nullptr){
            returnStatement += std::to_string(i) + " --> ";
            SymbolInfo* current = hashTable[i];
            while(current != nullptr){
                returnStatement += current->toString() + "  ";
                current = current->getNext();
            }
            returnStatement += '\n';
        }
    }

    return returnStatement;
}

ScopeTable* ScopeTable::getParent(){
    return parent;
}

int ScopeTable::getChildAmount(){
    return numberOfChild;
}

void ScopeTable::incrementChild(){
    numberOfChild++;
}
