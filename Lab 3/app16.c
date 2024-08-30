/*
Raduletu Petre-Horia
IA3 subgrupa1

Acest program este conceput cu ideea de a demonstra ce face functia execlp().Programul ruleaza un program auxiliar care afiseaza"Program rulat cu succes"

Programul trateaza : 
-erori la apelarea fara succes execlp


*/


#include <stdio.h>// Librarie standard Input/Output
#include <unistd.h>//acces familia de functii exec

int main() {

    char *command = "ExecutabilTest";
    
  
    execlp(command, command, NULL);

  
    perror("execlp");
    return 1;
}





/*
./app16
Program rulat cu succes


*/
