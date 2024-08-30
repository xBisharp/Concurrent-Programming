/*
Raduletu Petre-Horia
IA3 subgrupa 1

Acest program este implementat dupa instructiunile de la tema. Aceasta implementeaza functia shell() ce are ca scop replicarea unui shell facut intr-o forma manuala, functia system() ce are ca scop executarea comenzilor atribuite prin shell, si functia login() ce are ca scop validare credentialeleor gasite in credentials.txt 

programul acopera:
-comenzi gresite
-erori la deschiderea fisierelor
-


*/


#include <stdio.h>//Librarie standard input si output
#include <stdlib.h>// 
#include <string.h>//librarie standard pentru lucrul cu stringuri

#define MAX_COMMAND_LENGTH 1000



//system app
int systemApp(const char *command) {

    int status = system(command);

    return status;

}


//functia de shell
void shell() {

    char input[MAX_COMMAND_LENGTH];

    while (1) {

        printf("shell> ");

        fgets(input, MAX_COMMAND_LENGTH, stdin);

        input[strcspn(input, "\n")] = '\0'; 
        


        if (strcmp(input, "exit") == 0) {

            break;

        }



	//apelam functia system pentru realiza comenzile utilizatorului 
        int status = systemApp(input);
	//eroare la comanda
        if (status == -1) {

            perror("Error executing command");

        }
    }
}

void login(const char *username, const char *password) {

    FILE *credentials = fopen("/tmp/credentials.txt", "r");//instram in fisierul de credentiale


    if (credentials == NULL) {
    	//aruncam eroare daca fisierul nu paote sa fie accesat
        perror("Error opening credentials file");

        return;

    }



    char buffer[100];
    char *stored_username;
    char *stored_password;



    while (fscanf(credentials, "%s %s", buffer, buffer) != EOF) {

        stored_username = strtok(buffer, ",");

        stored_password = strtok(NULL, ",");



        if (strcmp(stored_username, username) == 0 && strcmp(stored_password, password) == 0) {

            printf("Login successful.\n");

            fclose(credentials);

            return;

        }

    }


    printf("Invalid username or password.\n");

    fclose(credentials);


}

int main() {

    char username[50];
    char password[50];

    // parola si usrname
    printf("Enter username: ");

    scanf("%s", username);


    printf("Enter password: ");

    scanf("%s", password);



    login(username, password);

    shell();//apelam shell

    return 0;
}
/*Cazuri Utilizare
./app21
Enter username: Macanel
Enter password: 123
Login successful.
shell> ls
app11	 app12	  app13    app14    app15.c  app21    ExecutabilTest
app11.c  app12.c  app13.c  app14.c  app16.c  app21.c  ExecutabilTest.c
shell> ^C




*/
