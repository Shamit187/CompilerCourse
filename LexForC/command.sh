flex -o $1.c $1.l
g++ $1.c ./ScopeTableClass/*.cpp -lfl -o $1.out
./$1.out $1.txt
rm $1.out $1.c
