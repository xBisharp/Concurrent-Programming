/*
Raduletu Petre Horia 
IA3 subgrupa 1

Acest program integreaza comanda --ps sau -p. Prin apelarea comenzii o sa fie afisate toate procesele active in sistem la momentul actual precum si identificatorii lor (PID, PPID, GID, EGID, UID, EUID, respectiv identificatorul de grup) accesand folderul /proc


Programul trateaza:
-comenzi incorecte
-erori la deschiderea fisierelor.

*/

#include <stdio.h>//librarie input output
#include <stdlib.h>//librarie standard 
#include <string.h>//librarie folosita pentru lucrul cu stringuri
#include <dirent.h>// librarie folosita pentru a accesa directoare 
#include <getopt.h>//librarie folosita pentru a accesa functiile getopt



//Aceasta functie are ca scop scrierea informatiilor legate de un proces
void printProcessInfo(const char *fieldName, const char *line) {
    
    if (strstr(line, fieldName) != NULL)printf("%s", line);

}


//Functia care intra propiu zis in fisierul proc si itereaza prin procese
void Processes(struct dirent *entry) {

    char path[PATH_MAX];

    snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);//am aflat de snpritf de pe stackoverflow de la o probblema similara 

    FILE *file = fopen(path, "r");//deschidem  fisierul

	
	//daca fisierul este incorect intoarcem eroare
    if (file == NULL) {
        perror("Error opening file");
        return;
    }


	
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
	
	//citim ce este in fisierul /proc si cu autorul functiei definite anterior si afisam
    while ((read = getline(&line, &len, file)) != -1) {
    
        printProcessInfo("Name:", line);
        printProcessInfo("Pid:", line);
        printProcessInfo("PPid:", line);
        printProcessInfo("Gid:", line);
        printProcessInfo("Uid:", line);
        printProcessInfo("Groups:", line);
    
    }

    printf("\n");
    free(line);//eliberam memoria din line
    fclose(file);//inchidem fisieru
}

int main(int argc, char *argv[]) {
    DIR *procDir;
    struct dirent *entry;
    
    //structura pentru comanda
    int opt= 0;

    int flag = 0 ;
    
    static struct option long_options[] = {
	{"ps",    no_argument, 0,  'p' },
	{0,0,0,0}
	
	};
	
	
	int long_index =0;
	while ((opt = getopt_long_only(argc, argv,"",long_options, &long_index )) != -1){

	//aceasi structura ca pana acum un switch.

        switch (opt) {
             case 'p':
             	flag =1;
             	
             	break;
                 
             default:
                 exit(EXIT_FAILURE);
        }
        }
	
    
    if(flag == 1 ){
	
	//deschidem fisierul
    procDir = opendir("/proc");
    if (procDir == NULL) {
        perror("Unable to open directory");
        return EXIT_FAILURE;
    }
	
	//cat timp se poate citi apelam functia Processes pentru fiecare element
    while ((entry = readdir(procDir)) != NULL) {
        if (atoi(entry->d_name) != 0) { 
           Processes(entry);
        }
    }
	
	//ichidem fisierul si iesim din program
    closedir(procDir);
    return EXIT_SUCCESS;}
    else {printf("Comanda necunoscuta");}
    
    }
/*
	CAZURI DE UTILIZARE 


./app3 --xx
./app3: unrecognized option '--xx' (comenzi gresite)



	
	Name:	systemd
Pid:	1
PPid:	0
PPid:	0
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kthreadd
Pid:	2
PPid:	0
PPid:	0
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	rcu_gp
Pid:	3
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	rcu_par_gp
Pid:	4
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	netns
Pid:	5
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kworker/0:0H-events_highpri
Pid:	7
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kworker/u8:0-flush-8:0
Pid:	8
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kworker/0:1H-kblockd
Pid:	9
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	mm_percpu_wq
Pid:	10
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	rcu_tasks_kthread
Pid:	11
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	rcu_tasks_rude_kthread
Pid:	12
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	rcu_tasks_trace_kthread
Pid:	13
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	ksoftirqd/0
Pid:	14
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	rcu_preempt
Pid:	15
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	migration/0
Pid:	16
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	idle_inject/0
Pid:	17
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	cpuhp/0
Pid:	19
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	cpuhp/1
Pid:	20
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	idle_inject/1
Pid:	21
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	migration/1
Pid:	22
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	ksoftirqd/1
Pid:	23
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kworker/1:0H-events_highpri
Pid:	25
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	cpuhp/2
Pid:	26
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	idle_inject/2
Pid:	27
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	migration/2
Pid:	28
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	ksoftirqd/2
Pid:	29
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kworker/2:0H-events_highpri
Pid:	31
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	cpuhp/3
Pid:	32
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	idle_inject/3
Pid:	33
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	migration/3
Pid:	34
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	ksoftirqd/3
Pid:	35
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kworker/3:0H-events_highpri
Pid:	37
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kdevtmpfs
Pid:	38
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	inet_frag_wq
Pid:	39
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kauditd
Pid:	40
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	khungtaskd
Pid:	41
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	oom_reaper
Pid:	43
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	writeback
Pid:	45
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kcompactd0
Pid:	46
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	ksmd
Pid:	47
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	khugepaged
Pid:	48
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kintegrityd
Pid:	49
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kblockd
Pid:	50
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	blkcg_punt_bio
Pid:	51
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	tpm_dev_wq
Pid:	52
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	ata_sff
Pid:	53
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	md
Pid:	54
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	edac-poller
Pid:	55
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	devfreq_wq
Pid:	56
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	watchdogd
Pid:	57
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kswapd0
Pid:	59
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	ecryptfs-kthread
Pid:	61
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kthrotld
Pid:	68
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	acpi_thermal_pm
Pid:	73
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	xenbus_probe
Pid:	74
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	scsi_eh_0
Pid:	75
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	scsi_tmf_0
Pid:	76
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	scsi_eh_1
Pid:	77
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	scsi_tmf_1
Pid:	78
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	vfio-irqfd-clea
Pid:	79
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kworker/1:1H-events_highpri
Pid:	81
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kworker/2:1H-kblockd
Pid:	82
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	mld
Pid:	83
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	ipv6_addrconf
Pid:	84
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 

Name:	kstrp
Pid:	89
PPid:	2
PPid:	2
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
Groups:	 







*/
