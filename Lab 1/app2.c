/* Raduletu Petre - Horia
*IA3 subgrupa 1


Acest program calculeaza aria si perimpetrul unui dreptunghi folosind getopt long
Contine comenzile area(-a), si perimeter(-p) care calculeaza folosind indicii lenght si breadth aria si permietrul unui dreptunghi, pentru a folosi cele doua comenzi este necesar sa stabilim care sunt cei doi indici dupa modelul aratat la inceputul rularii  prin functia print_usage

Aceasta aplicatie a fost folosita pentru a ma familiariza putin mai mult cu functiile getopt si este putin mai aplicativa decat app1


Trateaza :
--argumente incomplete 
--argumente gresite
--argumente inexistente
*/


#include <stdio.h>//librarie folosita pentru statusurile de iesire 
#include <stdlib.h>//librarie folosita aici pentru functia atoi(), folosita pentru a converti stringurile in integers
#include <getopt.h>//librarie folosita pentru a accesa functiile getopt


void print_usage() {
    printf("Usage: app2 [ap] -l num -b num (must be positive) \n");
}

int main(int argc, char *argv[]) {
int opt= 0;
int area = -1, perimeter = -1, bread = -1, length =-1;

//Definim structura
// doar lenght si bredth o sa aibe nevoie de o valoare acestea fiind necesare pentru a calcula cele doua valori
static struct option long_options[] = {
	{"area",      no_argument,       0,  'a' },
	{"perimeter", no_argument,       0,  'p' },
	{"length",    required_argument, 0,  'l' },
	{"bread",   required_argument, 0,  'b' },//(i used bread because it seemed funny :~) ) 
	{0,0,0,0}
 
 };


	//definim 
int long_index =0;
while ((opt = getopt_long_only(argc, argv,"",long_options, &long_index )) != -1) {//are acelasi rezultat ca while(1) if val == 1 doar ca mai corect
        switch (opt) {
             case 'a':
             	 
             	 area = 0;
                 break;
                 
             case 'p':
             
             	 perimeter = 0;
                 break;
                 
             case 'l':
             
             	 length = atoi(optarg); 
                 break;
                 
             case 'b':
             
             	 bread = atoi(optarg);
                 break;
                 
             default:
             
             	 print_usage(); 
                 exit(EXIT_FAILURE);// de data aceasta am folosit exit(EXIT_FAILURE) pentru ca este mult mai ok decat sa folosesc abort :D 
        }
    }
    //daca unul din cei doi indici lenght sau breadth este invalid(negativ) atunci iesim prin eroare si intoarcem prin print usage modul de folosire
    if (length <= -1 || bread <= -1) 
    {
        print_usage();
        exit(EXIT_FAILURE);
    }

    //Aici calculam aria
    if (area == 0) 
    {
        area = length * bread;
        printf("Area: %d\n",area);
    }

    //Aici calculam perimetrul 
    if (perimeter == 0) 
    {
        perimeter = 2 * (length + bread);
        printf("Perimeter: %d\n",perimeter);
    }
    return 0;
}


/*Cazuri de folosire : 
 AREA : 
 am incercat sa folosesc unul din numere cu valoare negativa si a intors rezultatul dorit(adica eroare)
./app2 --area -l -5 -b 7
Usage: app2 [ap] -l num -b num (must be positive)  

./app2 --area -l 5 -b -7
Usage: app2 [ap] -l num -b num (must be positive) 
<><><><><><><<><>><><><><><><><><><><>

am incercat cu valori pozitive si rezultatul este cel dorit
./app2 --area -l 5 -b 7
Area: 35

<><><><><><><<><>><><><><><><><><><><>

PERIMETER : 

am folosit indici potriviti si a intors un rezultat favorabil
./app2 -p -l 5 -b 7
Perimeter: 24

<><><><><><><<><>><><><><><><><><><><>

am incercat sa folosesc unul din numere cu valoare negativa si a intors rezultatul dorit( adica eroare) 
./app2 -p -l 5 -b -7
Usage: app2 [ap] -l num -b num (must be positive) 

./app2 -p -l -5 -b 7
Usage: app2 [ap] -l num -b num (must be positive) 
<><><><><><><<><>><><><><><><><><><><>
amii indici negativi si rezultatul este cel dorit (adica tot eroare)
./app2 -p -l -5 -b -7
Usage: app2 [ap] -l num -b num (must be positive) 


*/


