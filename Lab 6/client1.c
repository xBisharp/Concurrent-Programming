/*Raduletu Petre Horia
IA3 subgrupa 1


Acest program demonstreaza utilizarea socket-urilor pentru a crea un server care poate fi accesat de un client.Serverul poate oferiinformatii despre timpul de functionare, statistici despre sistem si poate executa comenzi shell.Se foloseste un port prestabilit pentru conexiunea client-server.Se foloseste un buffer pentru a citi mesajele de la client.Se foloseste un flag pentru a opri serverul la semnalul SIGINT sau SIGTERM.


 
Erori tratate :
- erori la crearea socket-ului
- erori la bind
- erori la recvfrom
-erori la sendto
-erori la deschiderea fisierelor de sistem
-erori la fork
-erori la exec
 */
 
#include <stdio.h> // Folosit pentru perror
#include <stdlib.h> // FOlosit pentru exit
#include <string.h> // strcmp, strlen
#include <sys/socket.h> // Folosit pentru  socket, sendto, recvfrom
#include <netinet/in.h> //Folosit pentru sockaddr_in, htons
#include <arpa/inet.h> // Folosit for inet_addr
#include <unistd.h> //close
#include <signal.h> //signal

#define SERVER_PORT 54321 // Port server
#define SERVER_IP "127.0.0.1" // IP  server
#define BUFFER_SIZE 1024 // Maximum messages

int sockfd;
struct sockaddr_in servaddr;




//Aceasta functie este folosita pentru tratarea pierderii de semnal(SIGINT)
void signal_handler(int sig) {

    if (sig == SIGINT) {
        
        const char* msg = "client_disconnect";
        
        
        sendto(sockfd, msg, strlen(msg), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        
        close(sockfd);
        
        
        printf("\nClient disconnected safely.\n");
        exit(0);
    }
}
// Aceasta functie printeaza raspunsul primit de la server
void get_server_response(int sockfd, struct sockaddr_in *servaddr) {
    
    char buffer[BUFFER_SIZE];
    int n;
    socklen_t len = sizeof(*servaddr);

    
    n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) servaddr, &len);
    
    
    if (n < 0) {
    
        perror("recvfrom error");
    
        return;
    }
    
    
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);
}

int main() {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    
    // Setam adresa serverului 
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

   
    // Inregistram handlerul pentru semnal
    signal(SIGINT, signal_handler);
   
   
   
   
    while (1) {

        char buffer[BUFFER_SIZE];
        printf("$");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  //Stergem caracterul pentru newline
        
        
        
        //Aici vedem daca userul vrea sa paraseasca aplicatia
        if (strcmp(buffer, "quit") == 0) {
            sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
            break;
        }
        //Trimitem mesaul la server
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        
        
        // Si aici primim raspunsul
        get_server_response(sockfd, &servaddr);
    }


	
    close(sockfd);//inchidem soketul
    return 0;
}


/*

Rez folosire :
./server
./client
$uptime
Server: Server is UP for 0d 5h 57min
$stats
Server: Load Avg: 1.28, 1.32, 1.36  CPU Usage: 0%  Memory Usage: Total: 12331688 kB, Used: 5733508 kB  
$cmd: ls
Server: client
client
client.c
server
server.c
$quit
 */
