/*
Raduletu Petre-Horia
IA3 sugrupa 1

Acest program este conceput sa calculeze aria si perimetrul unui cerc
Acesta contine optiunile --area(-a) si --perimeter(-p) care determina ce urmeaza sa fie calculat si optiunea --radius(-r) pentru a specifica raza cercului a carui perimetru sau arie urmeaza sa fie calculata 

Aceasta aplicatia a fost folosita pentru a ma familiariza cu libraria argp si este o continuare de la app2

Trateaza :
--argumente incomplete
--argumente gresite
--argumente inexistente

*/


#include <stdio.h> // Input output lirary
#include <stdlib.h>//folosita aici pentru functia atoi()
#include "argp.h"//liraria inclusa pentru a putea folosi argp


//scurta documentatie
static char doc[] = "Un exemplu de program care foloseste Argp ce calculeaza aria si perimetrul unui cerc";

static char args_doc[] = "-r RADIUS -p -a";


//aici definim optiunile care urmeaza sa fie folosite mai departe in executare
static struct argp_option options[] = {
    {"area", 'a', 0, 0, "Calculate area"},
    {"perimeter", 'p', 0, 0, "Calculate perimeter"},
    {"radius", 'r', "RADIUS", 0, "Specify radius"},
    {0}
};


//agest struct o sa fie folosit in main pentru a putea comunica cu parseopt
struct arguments {
    char *radius;
    int area, perimeter;
};


//Urmatoarea functie este cea care analizeaza argumentele date de utilizator
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'r':
            
            arguments->radius = arg;
            
            break;

        case 'a':
            
            arguments->area = 1;
            
            break;

        case 'p':
            
            arguments->perimeter = 1;
            
            break;

        case ARGP_KEY_ARG:
            
            return 0; 

        case ARGP_KEY_END:
            //daca argumentul radius este NULL programul indica o eroare
            if (arguments->radius == NULL)
                argp_failure(state, 1, 0, "Radius is required");
            
            break;

        default:
        
            return ARGP_ERR_UNKNOWN;
    }
    
    
    return 0;
    
    
}

static struct argp argp = { options, parse_opt, args_doc, doc };



int main(int argc, char **argv) {


    struct arguments arguments = {NULL, 0, 0};//Initializarea argumentelor cu valori implicite (pentru radius este nevoie de NULL)

	
    //call al functiei argp_parse pentru a analiza argumentele folosite de utilizator
    argp_parse(&argp, argc, argv, 0, 0, &arguments);


    //calculam aria si petimentrul in functie de radius
    int radius = atoi(arguments.radius);

    if (arguments.area) {
        float areaCircle = 3.14 * radius * radius;
        printf("Area: %f\n", areaCircle);
    }

    if (arguments.perimeter) {
        float perimeterCircle = 3.14 * 2 * radius;
        printf("Perimeter: %f\n", perimeterCircle);
    }

    return 0;
}

/*
Cazuri defolosire :

AREA :
./app3 -r 5 -a
Area: 78.500000

./app3  -a
app3: Radius is required

./app3 -r -5 -a
Area: 78.500000

/app3 -a -r 5
Area: 78.500000


<><><><><><><<><>><><><><><><><><><><>
PERIMETER :

./app3 -r -5 -p
Perimeter: -31.400000

./app3 -p
app3: Radius is required

./app3 -r 100 -p
Perimeter: 628.000000

./app3 -p -r 5
Perimeter: 31.400000


<><><><><><><<><>><><><><><><><><><><>
ALTELE :

./app3 -p -5 -a
./app3: invalid option -- '5'

./app3 -r 100 -p
Perimeter: 628.000000

./app3 -k
./app3: invalid option -- 'k'(pentru comportamente necuoscute)


*/

