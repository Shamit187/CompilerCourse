#include "SymbolTable.h"
#include <fstream>
#include <sstream>

std::string inputManagement(std::string& line, SymbolTable& symbolTable){
    std::string command;
    std::string secondCommand;
    std::string input1;
    std::string input2;
    std::string returnStatement = "";

    std::istringstream input(line);
    input >> command;

    if(command == "I")          //done
    {
        input >> input1 >> input2;
        symbolTable.insert(input1, input2, returnStatement);
    }
    else if(command == "L")     //done
    {
        input >> input1;
        symbolTable.lookup(input1, returnStatement);
    }
    else if(command == "D")
    {
        input >> input1;
        if(!symbolTable.remove(input1, returnStatement))
            return input1 + " not found";
    }
    else if(command == "P")     //done
    {
        input >> secondCommand;
        if(secondCommand == "A")
            returnStatement = symbolTable.printAllScopeTable();
        else if(secondCommand == "C")
            returnStatement = symbolTable.printCurrentScopeTable();
        else
            returnStatement = "Unknown command detected";
    }
    else if(command == "S")     //done
    {
        return symbolTable.enterScope();
    }
    else if(command == "E")     //done
    {
        return symbolTable.exitScope();
    }
    else if(command == "Q"){    //done
        return "";
    }

    return returnStatement;
}


int main(){
    unsigned int bucketSize;
    bool fileInputFlag = true;
    bool fileOutputFlag = false;
    std::string line = "";
    std::ifstream inputFile("input.txt", std::ios::in);
    std::ofstream outputFile("output.txt", std::ios::out);

    std::cout << "Press 1 for terminal input, Press 2 for file input >>";
    std::cin >> bucketSize;
    if(bucketSize == 1) fileInputFlag = false;

    std::cout << "Press 1 for terminal output, Press 2 for file output >>";
    std::cin >> bucketSize;
    if(bucketSize == 2) fileOutputFlag = true;

    if(fileOutputFlag){
        if (!outputFile) {
            std::cerr << "Error: output file could not be opened" << std::endl;
            exit(1);
        }
    }

    if(fileInputFlag) {
        if (!inputFile) {
            std::cerr << "Error: input file could not be opened" << std::endl;
            exit(1);
        }

        getline(inputFile, line);
        std::istringstream bufferInput(line);
        bufferInput >> bucketSize;
    }
    else{
        std::cout << "Bucket Size: ";
        std::cin >> bucketSize;
    }

    SymbolTable symbolTable(bucketSize);

    std::cout << std::endl;

    while(true)
    {
        if(fileInputFlag) {
            if(!getline(inputFile, line))
                break;
        }
        else {
            std::getline(std::cin, line);
            while (line == "")
                std::getline(std::cin, line);
        }
        std::string returnStatement = inputManagement(line, symbolTable);
        if (returnStatement == "") break;
        if(fileOutputFlag) {
            outputFile << "Input: " << line << std::endl;
            outputFile << "Output: " << std::endl;
            outputFile << returnStatement << std::endl << std::endl;
        }
        else {
            std::cout << "Input: " << line << std::endl;
            std::cout << "Output: " << std::endl;
            std::cout << returnStatement << std::endl << std::endl;
        }
    }
}