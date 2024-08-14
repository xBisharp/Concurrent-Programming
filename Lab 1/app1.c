/**
Raduletu Petre-Horia
IA3 subgrupa 1

Acest program ar trebui sa praseze corect comenzile --verbose, --add, --create,--append,--file, cat si formele lor scurte (-v,-a,-c,etc.) . Programul parseaza inclusiv optiuni ce necesita argumente (--delete sau -d). In final seateaza un flagul verbose_flag in cazul in care comanda  --verbose a fost furnizata

Am folosit aceasta aplicatie pentru a ma familiariza cu utilizarea functiei getopt, aceasta nefiind una foarte complexa.

Trateaza :
--argumente lipsa
--argumente invalide

*/

  
#include <stdio.h>// pentru input si output
#include <stdlib.h>//pentru abort()
#include <getopt.h>//pentru getopt_long()



//Flag pus de '--verbose'
static int verbose_flag;



int main(int argc, char **argv){

int val;
	static struct option long_options[]={
	//aceste doua optiuni lanseaza un flag
		{"verbose",no_argument, &verbose_flag,1},
		{"brief",no_argument,&verbose_flag,0},

	//aceste optiuni in schimb nu lanseasa un flag si sunt deoseite prin indici
		{"add",     no_argument,       0, 'a'},
		{"append",  no_argument,       0, 'b'},
		{"delete",  required_argument, 0, 'd'},
		{"create",  required_argument, 0, 'c'},
		{"file",    required_argument, 0, 'f'},
		{0,0,0,0}
		};

int option_index = 0;// getopt_long ar trebui sa stocheze aici indexul optiunii alese 

while (1){


	val = getopt_long(argc,argv,"abc:d:f:" , long_options, &option_index); 

	if(val == -1)break;// aici detectam momentul in care se termina instructiunile

	switch (val){

		case 0 :
//daca aici a aparut un flag nu ar mai trebui sa fie facut nimic
			if (long_options[option_index].flag != 0)break;
			printf ("option %s", long_options[option_index].name);

			if (optarg)printf (" with arg %s", optarg);

			printf ("\n");
			break;

// reailzam si restul cazurilor de functionare
		case 'a':
			puts ("option -a\n");
			break;
		
		case 'b':
			puts ("option -b\n");
			break;
	
		case 'c':
			printf ("option -c with value `%s'\n", optarg);
			break;
	
		case 'd':
			printf ("option -d with value `%s'\n", optarg);
			break;
	
		case 'f':
			printf ("option -f with value `%s'\n", optarg);
			break;
	
		case '?':
//aici getopt ar trebui sa raporteze eroarea
			break;

		default:
			abort();
}


}

//In loc sa raportam de fiecare data --verbose si --brief o sa raportam o singura data statusul lor  la final


if(verbose_flag)puts("verbose flag is set");


//printam restul argumentelor de pe linia de comanda care nu sunt optiuni 
if (optind < argc)
{
printf ("non-option ARGV-elements: ");

while (optind < argc)printf ("%s ", argv[optind++]);


putchar ('\n');

}

exit(0);

}


/**

In cazul in care programul primeste optiuni necunoscute cazul '?' este accesat si programul iese din switch si perite getopt sa proceseze eroarea intern


In cazul in care exista comportamente necunoscute cazul default este accesat si programul isi termina exectutarea 

Cazuri de executare : 
./app1
(nu intoarce nimic)

./app1 -a
option -a

./app1 -b
option -b

./app1 -d 10
option -d with value `10'

(in cazul in care una din optiunile ce necesita un argument nu il primesc se intoarce mesaul urmator
./app1 -c
./app1: option requires an argument -- 'c'

./app1 -c 5
option -c with value `5'

./app1 -f salut
option -f with value `salut'

./app1 -tw
./app1: invalid option -- 't'
./app1: invalid option -- 'w

*/

