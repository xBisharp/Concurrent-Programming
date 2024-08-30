/*
Raduletu Petre-Horia
IA3 subgrupa 1

Acest program este conceput dupa schema din tema si primeste de la tastatura prin optiunea -p un numar natural mai mic ca 20 si creaza schema aferenta plecand de la procesul 0

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
void childproc(int n) {
    
    for (int i = 1; i <= n; ++i) {
    
        pid_t pid = fork(); // Cream propriu-zis procesul 
    
        if (pid == -1) {
    
            perror("fork"); // Daca dam gres atunci intoracem o eraore
    
            exit(EXIT_FAILURE);
    
        } else if (pid == 0) {
    
            // Aici afisam procesul copil
    
            printf("Proces[%d] PID %d PPID %d\n", i, getpid(), getppid()); // Printam pid si ppid
    
            exit(EXIT_SUCCESS); 
    
        } else {
    
            //Procesul parinte asteapta 
            wait(NULL);
        
        }
    }
}



int main(int argc, char *argv[]) {
int opt= 0;

int nr_proc = 0;
//Definim structura

static struct option long_options[] = {
	{"procese",    required_argument, 0,  'p' },
	{0,0,0,0}
 
 };

int long_index =0;

while ((opt = getopt_long_only(argc, argv,"",long_options, &long_index )) != -1){



        switch (opt) {
             case 'p':
             	nr_proc = atoi(optarg);
             	
             	break;
                 
             default:
                 exit(EXIT_FAILURE);
        }
        }

if(  nr_proc>0 && nr_proc !=0 && nr_proc<=20){

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
    
            childproc(nr_proc);
    
            exit(EXIT_SUCCESS); 
    
        } else {
            // Asteptam ca procesele copil sa termine 
            wait(NULL);
    
        }
    
        exit(EXIT_SUCCESS); .
    
    } else {
       //Procesul initial asteapta ca procesul B sa termine 
    
        wait(NULL);
    }



} else {
printf("Numar incorect");
exit(EXIT_FAILURE);}





}


/*

CAZURI DE UTILIZARE 

./app1 --proces 2
Proces[A] PID 3734 PPID 2538
Proces[B] PID 3735 PPID 3734
Proces[0] PID 3736 PPID 3735
Proces[1] PID 3737 PPID 3736
Proces[2] PID 3738 PPID 3736



 ./app1 -p 5
Proces[A] PID 3692 PPID 2538
Proces[B] PID 3693 PPID 3692
Proces[0] PID 3694 PPID 3693
Proces[1] PID 3695 PPID 3694
Proces[2] PID 3696 PPID 3694
Proces[3] PID 3697 PPID 3694
Proces[4] PID 3698 PPID 3694


./app1 -p 3
Proces[A] PID 3686 PPID 2538
Proces[B] PID 3687 PPID 3686
Proces[0] PID 3688 PPID 3687
Proces[1] PID 3689 PPID 3688
Proces[2] PID 3690 PPID 3688

./app1 -p -1
Numar incorect


./app1 --test 2
./app1: unrecognized option '--test'

./app1 -t 2
./app1: unrecognized option '-t'

./app1 -p 20
Proces[A] PID 3757 PPID 2538
Proces[B] PID 3758 PPID 3757
Proces[0] PID 3759 PPID 3758
Proces[1] PID 3760 PPID 3759
Proces[2] PID 3761 PPID 3759
Proces[3] PID 3762 PPID 3759
Proces[4] PID 3763 PPID 3759
Proces[5] PID 3764 PPID 3759
Proces[6] PID 3765 PPID 3759
Proces[7] PID 3766 PPID 3759
Proces[8] PID 3767 PPID 3759
Proces[9] PID 3768 PPID 3759
Proces[10] PID 3769 PPID 3759
Proces[11] PID 3770 PPID 3759
Proces[12] PID 3771 PPID 3759
Proces[13] PID 3772 PPID 3759
Proces[14] PID 3773 PPID 3759
Proces[15] PID 3774 PPID 3759
Proces[16] PID 3775 PPID 3759
Proces[17] PID 3776 PPID 3759
Proces[18] PID 3777 PPID 3759
Proces[19] PID 3778 PPID 3759
Proces[20] PID 3779 PPID 3759


./app1 -p 21
Numar incorect


*/


