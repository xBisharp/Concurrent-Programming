/*
Raduletu Petre-Horia
IA3 subgrupa1

Acest program este conceput cu ideea de a demonstra ce face functia execvp().Programul ruleaza un program auxiliar care afiseaza"Program rulat cu succes"

Programul trateaza : 
-erori la apelarea fara succes execvp


*/

#include <stdio.h>// Librarie standard Input/Output
#include <unistd.h>//acces familia de functii exec

int main() {
    char *args[] = {"ExecutabilTest", NULL}; 
    
    execvp(args[0], args); 


    perror("execvp");
    return 1;
}

/*
./app12
Program rulat cu succes

*/
