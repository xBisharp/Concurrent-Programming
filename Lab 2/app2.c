/*
Raduletu Petre-Horia
IA3 subgrupa 1

Acest program este conceput sub schema primita la tema. Similar cu app1 aceasta tema primeste de la tastatura prin optiunea -p un numar natural mai mic ca 20 si creaza schema aferenta plecand de la procesul 0, in plus poate primi un nou argument prin optiunea -s si creaza un numar de subprocese, numar primit de la tastatura

Programul trateaza:
- argumente incorecte
- valori incorecte 
-optiuni gresite


*/

#include <stdio.h>   // Lirarie pentru input output
#include <stdlib.h>  // Librarie introdusa pentru exit si atoi
#include <unistd.h>  //Pentru fork() getpid() si getppid()
#include <sys/wait.h> // Pentru functia wait
#include <getopt.h>//librarie folosita pentru a accesa functiile getopt

// Am facut aceasta functie pentru a crea un numar dea definit de procese
void childproc(int n,int s) {
    for (int i = 1; i <= n; ++i) {
    
        pid_t pid = fork(); // Cream propriu-zis procesul 
    
        if (pid == -1) {
    
            perror("fork"); // Daca dam gres atunci intoracem o eroare
    
            exit(EXIT_FAILURE);
    
        } else if (pid == 0) {
            // Aici afisam procesul copil
    
            printf("Proces[%d] PID %d PPID %d\n", i, getpid(), getppid()); // Printam pid si ppid
    
    
            
            //cream subprocesele
            pid_t subPid;
            for (int j = 1; j <= s; ++j) {
                subPid = fork();
    
                if (subPid == -1) {
    
                    perror("fork");
    
                    exit(EXIT_FAILURE);
    
                } else if (subPid == 0) {
                    
                    printf("Process[%d.%d] PID %d PPID %d\n", i, j, getpid(), getppid());
    
    
                    // Nu se mai creaza procese daca am ajuns la final
    
                    if (j == s) exit(EXIT_SUCCESS);
    
                } else {
                    // Parintele asteapta restul copiilor sa termine 
                    wait(NULL);
                    
                    exit(EXIT_SUCCESS);
                }
            }
            
            
            
            exit(EXIT_SUCCESS); 
        } else {
            //Procesul parinte asteapta 
            wait(NULL);
        }
    }
}



int main(int argc, char *argv[]) {
int opt= 0;

int nr_proc = 0,nr_sub=0,x=0;

//Definim structura
static struct option long_options[] = {
	{"procese",    required_argument, 0,  'p' },
	{"sub",    required_argument, 0,  's' },
	{0,0,0,0}
 
 };

int long_index =0;

while ((opt = getopt_long_only(argc, argv,"",long_options, &long_index )) != -1){



        switch (opt) {
             case 'p':
             	nr_proc = atoi(optarg);
             	
             	break;
                 
             case 's':
             	x = 1;
             	nr_sub = atoi(optarg);
             	
             	break; 
             default:
                 exit(EXIT_FAILURE);
        }
        }

if(  nr_proc>0 && nr_proc !=0 && nr_proc<=20 ){
if(x==1)if(nr_sub <0 && nr_sub>20){


printf("Numar incorect");


exit(EXIT_FAILURE);}



printf("Proces[A] PID %d PPID %d\n", getpid(), getppid());


pid_t pid_b = fork();


if (pid_b == -1) {

        perror("fork"); // Daca nu merge fork iesim prin eroare

        exit(EXIT_FAILURE);

    } else if (pid_b == 0) {
        
        printf("Proces[B] PID %d PPID %d\n", getpid(), getppid());
        
        
        
        pid_t pid_d = fork();

        if (pid_d == -1) {

            perror("fork"); // Daca nu merge fork iesim prin eroare

            exit(EXIT_FAILURE);

        } else if (pid_d == 0) {

            printf("Proces[0] PID %d PPID %d\n", getpid(), getppid());


            // Dam call la functia care creaza copii 

            childproc(nr_proc,nr_sub);

            exit(EXIT_SUCCESS); 
        } else {
            // Asteptam ca procesele copil sa termine 
            wait(NULL);

        }

        exit(EXIT_SUCCESS); 

    } else {
       //Procesul initial asteapta ca procesul B sa termine 

        wait(NULL);

    }



} else {
printf("Numar incorect");

exit(EXIT_FAILURE);}


}

/*
CAZURI DE FOLOSIRE 

/app2 -p 3 -s 5
Proces[A] PID 4234 PPID 2538
Proces[B] PID 4235 PPID 4234
Proces[0] PID 4236 PPID 4235
Proces[1] PID 4237 PPID 4236
Process[1.1] PID 4238 PPID 4237
Process[1.2] PID 4239 PPID 4238
Process[1.3] PID 4240 PPID 4239
Process[1.4] PID 4241 PPID 4240
Process[1.5] PID 4242 PPID 4241
Proces[2] PID 4243 PPID 4236
Process[2.1] PID 4244 PPID 4243
Process[2.2] PID 4245 PPID 4244
Process[2.3] PID 4246 PPID 4245
Process[2.4] PID 4247 PPID 4246
Process[2.5] PID 4248 PPID 4247
Proces[3] PID 4249 PPID 4236
Process[3.1] PID 4250 PPID 4249
Process[3.2] PID 4251 PPID 4250
Process[3.3] PID 4252 PPID 4251
Process[3.4] PID 4253 PPID 4252
Process[3.5] PID 4254 PPID 4253

./app2 -p 3 -s -1
Numar incorect


./app2 -p 3 -t 2
./app2: unrecognized option '-t'



./app2 -t 3 -s 2
./app2: unrecognized option '-t'

./app2 -p - 3 -s 5
Numar incorect


./app2 -p 3
Proces[A] PID 4394 PPID 2538
Proces[B] PID 4395 PPID 4394
Proces[0] PID 4396 PPID 4395
Proces[1] PID 4397 PPID 4396
Proces[2] PID 4398 PPID 4396
Proces[3] PID 4399 PPID 4396

./app2 --procese 3 --sub 3
Proces[A] PID 4465 PPID 2538
Proces[B] PID 4466 PPID 4465
Proces[0] PID 4467 PPID 4466
Proces[1] PID 4468 PPID 4467
Process[1.1] PID 4469 PPID 4468
Process[1.2] PID 4470 PPID 4469
Process[1.3] PID 4471 PPID 4470
Proces[2] PID 4472 PPID 4467
Process[2.1] PID 4473 PPID 4472
Process[2.2] PID 4474 PPID 4473
Process[2.3] PID 4475 PPID 4474
Proces[3] PID 4476 PPID 4467
Process[3.1] PID 4477 PPID 4476
Process[3.2] PID 4478 PPID 4477
Process[3.3] PID 4479 PPID 4478


*/

