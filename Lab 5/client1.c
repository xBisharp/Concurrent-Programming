/*
Raduletu Petre Horia
IA3 sugrupa 1


Acest program este facut dupa specificatiile fisierului Tema_5 si are ca scop simularea unui client care foloseste socket pentru a comunica cu partea de server. Clientul permite utilizatorului să interacționeze cu un server utilizând un meniu cu mai multe opțiuni. Aceste opțiuni includ:Trimiterea unui Mesaj,Cererea Orașului Curent,Cererea Utilizatorului Curent,Executarea unei Comenzi,Deconectarea
Utilizatorul poate alege oricare dintre aceste opțiuni și să interacționeze cu serverul în funcție de cerințele sale


Programul trateaza :

-Erori de Conectare
-Erori de Trimitere și Primire a Datelor
-Erori de Autentificare
-Erori de Execuție a Comenzii
-Erori de Deconectare
*/
#include <stdio.h>//I/O service
#include <stdlib.h>//lib standard
#include <string.h>//functii de string 
#include <sys/socket.h>// pentru socket
#include <arpa/inet.h>//
#include <unistd.h>//
#include <pwd.h> //pentru user
#include <time.h>// pentru ctime()

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

void authenticate(int client_socket) {
    char username[100];
    char password[100];
    
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    
    send(client_socket, username, strlen(username) + 1, 0);
    send(client_socket, password, strlen(password) + 1, 0);
}

void execute_command(int client_socket) {
    char command[BUFFER_SIZE];

    printf("Enter command: ");
    getchar(); // Clear input buffer
    fgets(command, BUFFER_SIZE, stdin);


    send(client_socket, command, strlen(command), 0);

  
    char buffer[BUFFER_SIZE];
    printf("Output from server:\n");
    while (recv(client_socket, buffer, BUFFER_SIZE, 0) > 0) {
        printf("%s", buffer);
    }
}

void send_message(int client_socket) {

    printf("Send a Message option selected.\n");
}

void request_current_time(int client_socket) {

    printf("Request Current Time option selected.\n");
}

void request_current_user(int client_socket) {
    
    printf("Request Current User option selected.\n");
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Cream socket 
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    //Structura server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    // Ne conectam la server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    // Facem autentificarea
    authenticate(client_socket);

    // Un loop simplu pentru meniu 
    int choice;
     char buffer[BUFFER_SIZE];
    do {
        printf("\nMenu:\n");
        printf("1. Send a Message\n");
        printf("2. Request Current Time\n");
        printf("3. Request Current User\n");
        printf("4. Execute a Command\n");
        printf("5. Disconnect\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

	// switch pentru optiuni
        switch (choice) {
            case 1:
    
                printf("Received request: Send a Message\n");
                if (recv(client_socket, buffer, BUFFER_SIZE, 0) < 0) {
                    perror("Error receiving message from client");
                    exit(EXIT_FAILURE);
                }
                printf("Message from client: %s\n", buffer);
              
            case 2:
                printf("Received request: Current Time\n");
                time_t now;
                time(&now);
                snprintf(buffer, sizeof(buffer), "%s", ctime(&now));
                send(client_socket, buffer, strlen(buffer), 0);
                break;
            case 3:
                printf("Received request: Current User\n");
                struct passwd *pw;
                pw = getpwuid(getuid());
                snprintf(buffer, sizeof(buffer), "%s", pw->pw_name);
                send(client_socket, buffer, strlen(buffer), 0);
                break;
            case 4:
                execute_command(client_socket);
                break;
            case 5:
                printf("Disconnecting...\n");
                close(client_socket);
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);

    return 0;
}

/*
Metode de utilizare 

./client 


Menu:
1. Send a Message
2. Request Current Time
3. Request Current User
4. Execute a Command
5. Disconnect
Enter your choice:

Aici alegem optiunea iar clientul trimite la server 


Enter your choice: 1
Enter message: Hello, server!


Enter your choice: 2
Tue Apr 04 10:14:00 2024


Enter your choice: 3
Current user: user 



Enter your choice: 4
app1.c
server
client1.c
client

Choice 5 inchide conexiunea




*/

