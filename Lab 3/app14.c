/*
Raduletu Petre-Horia
IA3 subgrupa1

Acest program este conceput cu ideea de a demonstra ce face functia execl().Programul ruleaza un program auxiliar care afiseaza"Program rulat cu succes"

Programul trateaza : 
-erori la apelarea fara succes execl


*/


#include <stdio.h>// Librarie standard Input/Output
#include <unistd.h>//acces familia de functii exec



int main() {
   
    char *command = "ExecutabilTest";
    
 
    execl(command, command, NULL);

  
    perror("execl");
    return 1;
}

/*
./app14
Program rulat cu succes


*/
