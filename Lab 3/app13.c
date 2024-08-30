/*
Raduletu Petre-Horia
IA3 subgrupa1

Acest program este conceput cu ideea de a demonstra ce face functia execve().Programul ruleaza un program auxiliar care afiseaza"Program rulat cu succes"

Programul trateaza : 
-erori la apelarea fara succes execve


*/
#include <stdio.h>// Librarie standard Input/Output
#include <unistd.h>//acces familia de functii exec

int main() {
    char *args[] = {"ExecutabilTest", NULL}; 
    char *env[] = {NULL};//nu este nimic de specificat la environment  
    
    execve(args[0], args, env);

    
    perror("execve");
    return 1;
}

/*
./app13
Program rulat cu succes

*/
