/*
Raduletu Petre Horia
IA3 subgrupa 1

Aceasta aplicatie este conceputa dupa modelul oferit la fisierul Tema_5. Aceasta aplicatie simuleaza un server care comunica cu un client(maxim 5). Serverul este responsabil pentru gestionarea cererilor primite de la client și pentru furnizarea de răspunsuri adecvate în funcție de cerințele clientului. Iată o scurtă descriere a ceea ce face serverul:Acceptă conexiuni,Primește cereri de la clienți,Procesează cererile,Furnizează răspunsuri,Menține conexiunea cu clienții

Programul trateaza :

-Erori la input
-Erori la credentiale
-erori ale conectivitatii
-Erori ale comenzilor tratate
-Erori

*/

#include <stdio.h>//I/O service
#include <stdlib.h>//lib standard
#include <string.h>//functii de string 
#include <sys/socket.h>// pentru socket
#include <arpa/inet.h>//
#include <unistd.h>//
#include <pwd.h> //pentru user
#include <time.h>// pentru ctime()

#define SERVER_PORT 12345
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char username[100];
    char password[100];
    char received_username[100];
    char received_password[100];

    // PReluam userul si parola
    recv(client_socket, received_username, sizeof(received_username), 0);
    recv(client_socket, received_password, sizeof(received_password), 0);

    //Yes indeed it is not the temp file
    strcpy(username, "user");
    strcpy(password, "pass");

    // Verificare username si parola
    if (strcmp(received_username, username) == 0 && strcmp(received_password, password) == 0) {
        printf("Authentication successful.\n");
        // Autentificare reusita intram in optiunile tratate
        while (1) {
            int choice;
            char buffer[BUFFER_SIZE];
            
            // Preluam alegerea din client
            if (recv(client_socket, &choice, sizeof(choice), 0) < 0) {
                perror("Error receiving choice from client");
                exit(EXIT_FAILURE);
            }

            switch (choice) {
                case 1:
                    // Printam mesaul
                    printf("Received request: Send Message\n");
                    break;
                case 2:
                    // Printam timpul
                    printf("Received request: Current Time\n");
                    time_t now;
                    time(&now);
                    snprintf(buffer, sizeof(buffer), "%s", ctime(&now));
                    send(client_socket, buffer, strlen(buffer), 0);
                    break;
                case 3:
                    // Printam user
                    printf("Received request: Current User\n");
                    struct passwd *pw;
                    pw = getpwuid(getuid());
                    snprintf(buffer, sizeof(buffer), "%s", pw->pw_name);
                    send(client_socket, buffer, strlen(buffer), 0);
                    break;
                case 4:
                    // Executam comanda
                    printf("Received request: Execute Command\n");
                    // aici preluam
                    if (recv(client_socket, buffer, BUFFER_SIZE, 0) < 0) {
                        perror("Error receiving command from client");
                        exit(EXIT_FAILURE);
                    }
                    // aici executam propiu-zis
                    FILE *fp = popen(buffer, "r");
                    if (fp == NULL) {
                        perror("Error executing command");
                        exit(EXIT_FAILURE);
                    }
                    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
                        send(client_socket, buffer, strlen(buffer), 0);
                    }
                    pclose(fp);
                    break;
                case 5:
                    // Disconnect
                    printf("Client disconnected.\n");
                    close(client_socket);
                    exit(EXIT_SUCCESS);
                default:
                    printf("Invalid choice received from client.\n");
            }
        }
    } else {
        printf("Authentication failed. Closing connection.\n");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Cream socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Adresa pentru server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // legam socetul 
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    //Ascultam pentru potentiale conexiuni
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        // Acceptam conexiunile
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }

        printf("New connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));


        handle_client(client_socket);
    }

    return 0;
}
/*
Cazuri utilizare

./server
Server listening on port 12345...
New connection accepted from 127.0.0.1:37246
Authentication failed. Closing connection.



*/
