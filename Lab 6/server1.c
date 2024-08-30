/*Raduletu PetreHoria
IA3 sugrupa 1



Acest program demonstreaza utilizarea socket-urilor pentru a crea un server care poate fi accesat de un client.Serverul poate oferi informatii despre timpul de functionare, statistici despre sistem si poate executa comenzi shell.Se foloseste un port prestabilit pentru conexiunea client-server.Se foloseste un buffer pentru a citi mesajele de la client.Se foloseste un flag pentru a opri serverul la semnalul SIGINT sau SIGTERM.



   
Acest program trateaza :
- erori la crearea socket-ului
-erori la bind
-erori la recvfrom
-erori la sendto
-erori la deschiderea fisierelor de sistem
-erori la fork
-erori la exec
 
 
 */
#include <stdio.h> // Pentru perror
#include <stdlib.h> // PEntru exit
#include <string.h> //strcmp, strlen
#include <sys/socket.h> // Pentru socket, sendto, recvfrom
#include <netinet/in.h> // Pentru sockaddr_in, htons
#include <unistd.h> //close
#include <sys/wait.h> //Pentru waitpid
#include <signal.h> //Pentru signal

#define PORT 54321 // Port server
#define BUFFER_SIZE 2048 // Maximum messages

volatile sig_atomic_t keep_running = 1; // Un flag pentru a tine serverul in functiune (thanks stackoverflow)

void sig_handler(int _) {
    (void)_;  //Paramentru inutilizat (thanks stackoverflow(again))
    keep_running = 0;
}




//Functie ca sa transmitem raspunsul la client
void send_response(int sockfd, struct sockaddr_in *cliaddr, socklen_t clilen, const char *message) {


    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)cliaddr, clilen);


}



// Aceasta functie executa comenzile si le trimite mai departe la client 
int adapted_custom_system(int sockfd, struct sockaddr_in *cliaddr, socklen_t clilen, const char *command) {
    
    int pipefds[2];
    pipe(pipefds);

    pid_t pid = fork();
    
    
    if (pid == -1) {
     
        perror("fork failed");
     
        return -1;
    
    
    }else if (pid == 0) {
        // Trimitem stdout si stderr la pipe
        close(pipefds[0]);
        dup2(pipefds[1], STDOUT_FILENO);
        dup2(pipefds[1], STDERR_FILENO);
        close(pipefds[1]);



        //Executam comanda in procesul copil
        execlp("/bin/sh", "sh", "-c", command, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else {
        // Inchidem write end-ul in parinte 
        close(pipefds[1]);




        //Citim output-ul
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = read(pipefds[0], buffer, sizeof(buffer) - 1);
        
        
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Terminam string
            send_response(sockfd, cliaddr, clilen, buffer); // Trimitem output la client 
            
            
            
        } else {
        
        
            send_response(sockfd, cliaddr, clilen, "Failed to capture command output");
        
        
        }
        close(pipefds[0]);

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}




// Functie pentru uptime request
void handle_uptime_request(int sockfd, struct sockaddr_in *cliaddr, socklen_t clilen) {
   
    
    
     // Citim uptime din /proc
    double uptime_seconds;
    FILE *uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL) {
        send_response(sockfd, cliaddr, clilen, "Failed to get uptime");
        return;
    }
    
    
    
    fscanf(uptime_file, "%lf", &uptime_seconds);
    fclose(uptime_file);
    
    

    int days = (int)uptime_seconds / (24 * 3600);
    uptime_seconds -= days * (24 * 3600);
    int hours = (int)uptime_seconds / 3600;
    uptime_seconds -= hours * 3600;
    int minutes = (int)uptime_seconds / 60;
    
    
    
    // Stringul pentru uptime
    char uptime_info[BUFFER_SIZE];
    snprintf(uptime_info, BUFFER_SIZE, "Server is UP for %dd %dh %dmin", days, hours, minutes);
    
   //trimitem uptime la client 
    send_response(sockfd, cliaddr, clilen, uptime_info);
   
}



void handle_stats_request(int sockfd, struct sockaddr_in *cliaddr, socklen_t clilen) {


    char response[2048];

    // Load averages
    float load1, load5, load15;
    FILE *fp = fopen("/proc/loadavg", "r");
    if (fp != NULL) {

        fscanf(fp, "%f %f %f", &load1, &load5, &load15);

        fclose(fp);

    } else {
 
        strcpy(response, "Error reading load averages");
 
        send_response(sockfd, cliaddr, clilen, response);
 
 
        return;
    }

 
 
    // CPU usage
    long double a[4], b[4], loadavg;
    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%*s %Lf %Lf %Lf %Lf", &a[0], &a[1], &a[2], &a[3]);
    fclose(fp);
    
    sleep(1);  // asteapta o secunnda
    fp = fopen("/proc/stat", "r");
    fscanf(fp, "%*s %Lf %Lf %Lf %Lf", &b[0], &b[1], &b[2], &b[3]);
    fclose(fp);
    
    
    
    loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));
    int cpu_usage = (int)(loadavg * 100); // CPU usage in %

    
    
    // Memory usage
    int total_memory, free_memory, buffers, cached, sreclaimable, shmem;
    fp = fopen("/proc/meminfo", "r");
    char line[256];
    
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "MemTotal:", 9) == 0) sscanf(line, "MemTotal: %d kB", &total_memory);
        else if (strncmp(line, "MemFree:", 8) == 0) sscanf(line, "MemFree: %d kB", &free_memory);
        else if (strncmp(line, "Buffers:", 8) == 0) sscanf(line, "Buffers: %d kB", &buffers);
        else if (strncmp(line, "Cached:", 7) == 0) sscanf(line, "Cached: %d kB", &cached);
        else if (strncmp(line, "SReclaimable:", 13) == 0) sscanf(line, "SReclaimable: %d kB", &sreclaimable);
        else if (strncmp(line, "Shmem:", 6) == 0) sscanf(line, "Shmem: %d kB", &shmem);
    }
    
    fclose(fp);
    
    
    int used_memory = total_memory - free_memory - buffers - cached - sreclaimable + shmem;  // Aproximeaza memoria utilizata
    
    
    // Format pentru informatii
    snprintf(response, sizeof(response),"Load Avg: %.2f, %.2f, %.2f  CPU Usage: %d%%  Memory Usage: Total: %d kB, Used: %d kB",load1,load5, load15, cpu_usage, total_memory, used_memory);
    
    
    send_response(sockfd, cliaddr, clilen, response);//trimitem raspundul la client
   
}
//
void handle_command_execution(int sockfd, struct sockaddr_in *cliaddr, socklen_t clilen, const char *command) {
    // Update pentru functia de executie
    adapted_custom_system(sockfd, cliaddr, clilen, command);
}


int main() {


    // Setup pentru signal handler
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    // Creare socket
    
    
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    //Bind la socket si sv adress
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    
    
    // Structura sv
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    
    
    // Bind la socket si adresa
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Main loop pentru handle
    while (keep_running) {
        char buffer[BUFFER_SIZE];
        socklen_t len = sizeof(cliaddr);

        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
        
            perror("recvfrom failed");
            
            if (!keep_running) {
            
                break;  //exit loop la quit messaje
                
            }
            
            continue;
        }
        buffer[n] = '\0';  //Termina string

        //Cauta discconnect sau quit
        if (strcmp(buffer, "client_disconnect") == 0) {
            printf("Client disconnected.\n");
            break;
        }
        else if (strcmp(buffer, "quit") == 0) {
            printf("Quit command received, shutting down.\n");
            break;
        }


        //Parseaza comanda si trece prin handler
        if (strcmp(buffer, "uptime") == 0) {
            handle_uptime_request(sockfd, &cliaddr, len);
            printf("::: Sending current up time to client...\n");
            
            
        } else if (strcmp(buffer, "stats") == 0) {
            
            handle_stats_request(sockfd, &cliaddr, len);
            
            printf("::: Sending current stats to client...\n");
        } else if (strncmp(buffer, "cmd:", 4) == 0) {
           
           
            // Executa comanda
            // Format obligatoriu"cmd: command"
            if (strlen(buffer) > 4) {
                handle_command_execution(sockfd, &cliaddr, len, buffer + 4);
                printf("::: Sending some command output to client...\n");
            }
            
            
        } else {
            //Mesaj eraore pentru comenzi invalide
            char error_msg[BUFFER_SIZE] = "Invalid command";
            sendto(sockfd, error_msg, strlen(error_msg), 0, (struct sockaddr *)&cliaddr, len);
        }
    }
    //Inchidem socet
    printf("Shutting down server...\n");
    close(sockfd);
    return 0;
}

/*

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
