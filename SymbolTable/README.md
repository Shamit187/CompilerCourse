# Symbol Table
## A simple Symbol Table for the compiler project
*Bangladesh University of Engineering and Technology <br>
Computer Science and Engineering Department <br>
Undergrad Course: #CSE310 <br>*

**Notes.**
  1.  For file input, you need to create a text file with name ```"input.txt"``` in the same directory of your ```.exe``` file
  2.  For ```std::sin``` input, make sure that every command is in single line
  3.  For file output, the generated file will have the name ```"output.txt"```, and will be in the same directory of your ```.exe``` file
  4.  Almost every method is overriden with a similar method that takes in a string to write the log of that method. For convinient using, use the non overriden ones
  5.  For new addition, do the following
      -  Create new member in ```SymbolInfo```
      -  Generate getter setter in ```SymbolInfo```
      -  Add new parameters in ```SymbolInfo.constructor , ScopeTable.insert , SymbolTable.insert```
