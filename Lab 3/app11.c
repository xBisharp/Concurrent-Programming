/*
Raduletu Petre-Horia
IA3 subgrupa1

Acest program este conceput cu ideea de a demonstra ce face functia execv().Programul ruleaza un program auxiliar care afiseaza"Program rulat cu succes"

Programul trateaza : 
-erori la apelarea fara succes execv


*/
#include <stdio.h>// Librarie standard Input/Output
#include <unistd.h>//acces familia de functii exec

int main() {
    
    char *args[] = {"./ExecutabilTest", NULL}; 
	
    execv(args[0],args);
    
    
    perror("execv");
    return 1;
}

/*
CAZURI UTILIZARE:

./app11
Program rulat cu succes


*/
