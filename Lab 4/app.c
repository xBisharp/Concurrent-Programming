/*
Raduletu Petre-Horia
IA3 subgrupa 1

Acest program este conceput dupa specificatiile de la tema 4 

Acest program trateaza:
-erori pentru fisiere lipsa
-erori pentru fisiere fara permisiuni

*/
#include <stdio.h>//pentru inpt output
#include <stdlib.h>//
#include <errno.h>//pentru errno
#include <dirent.h>//pentru DIR
#include <string.h>//pentru string
#include <stddef.h>//pentru size_t
#include <unistd.h>//pentru open close etc
#include <sys/types.h>//
#include <sys/wait.h>//pentru sincronizari
#include <fcntl.h>//pentru a manipula descriptorii
#include <pthread.h> //pentru pthread_create, pthread_join
#include <pwd.h> // getpwduid
#include <sys/stat.h>//pentru stat

#define CHAR_LIMIT 64
#define FILE_LIMIT (1<<16) //stackoverflow

extern int errno;

typedef struct in_file
{
   char* file_path;
   char* buffer;
   size_t size;
}in_file_t;


typedef struct out_file 
{ 
   char* file_path;
   char* buffer;
   size_t size;
}out_file_t;



//definim my_read
size_t my_read(char* file_name, char* buffer, size_t size)
{
    int fd = open(file_name, O_RDONLY); //deschidem fisierul

    if(fd < 1) // daca nu exista fisierul sau nu are permisiuni
    {
        write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
        write(STDERR_FILENO, "\n", 1);
        exit(EXIT_FAILURE);
    }

    char* temp = malloc(size);
    int seek = 0;
    int read_size = 0;

    while((read_size = read(fd, temp, size)) > 0) //citim pas cu pas
    {
        strncpy(buffer + seek, temp, read_size);
        seek += read_size;
    }

    buffer[seek] = '\0';

    close(fd);
    free(temp);

    return seek;
}


//definim my_write
size_t my_write(char* file_name, char* buffer, size_t size)
{
    int fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0777); //deschidem fisierul

    if(fd < 1) // daca nu exista fisierul sau nu are permisiuni
    {
        write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
        write(STDERR_FILENO, "\n", 1);
        exit(EXIT_FAILURE);
    }

    char* temp = malloc(CHAR_LIMIT);
    int remaining_size = 0;
    int current_size = 0;
    int written_size = 0;

    while(written_size < size)
    {
        remaining_size = size - written_size;
        current_size = remaining_size < CHAR_LIMIT? remaining_size : CHAR_LIMIT; 
        memcpy(temp, buffer + written_size, current_size); // copiem stringul actual
        write(fd, temp, current_size); // scriem in fisier
        written_size += current_size;  // marim marimea scrisa
    }

    close(fd);//inchidem fisierul
    free(temp);//eliberam memoria filosita la temp

    return written_size;
}

//definim in_thread
void* in_thread(in_file_t* file)
{

    fprintf(stdout, "Size:%d\n Reading...\n", file->size);
    my_read(file->file_path, file->buffer, file->size);
    fprintf(stdout, "Finished Reading\n", file->size);
    return file;
}


//definim out_thread
void* out_thread(in_file_t* file)
{
    fprintf(stdout, "Size:%d\n Writing...\n", file->size);
    my_write(file->file_path, file->buffer, file->size);
    fprintf(stdout, "Finished Writing\n", file->size);
    return file;
}



int main(int argc, char** argv)
{
    char* buffer = malloc(FILE_LIMIT);
    int read_limit = 0;
    char in_file_name[64], out_file_name[64];
    printf("IN FILE:");scanf("%s", in_file_name);
    printf("OUT FILE:");scanf("%s", out_file_name);
    printf("BYTES:");scanf("%d", &read_limit);
    
    
    //cream thread 1 
    in_file_t in_file = {in_file_name, buffer, read_limit};
    pthread_t thr1;
    pthread_create(&thr1, NULL, (void*)in_thread, (void*)&in_file);
    pthread_join(thr1, NULL);
    
    
    //cream thread 2
    out_file_t out_file = {out_file_name, buffer, read_limit};
    pthread_t thr2;
    pthread_create(&thr2, NULL, (void*)out_thread, (void*)&out_file);
    pthread_join(thr2, NULL);



    free(buffer);

    return 0;
}
/*

I have accepted my deafeat long ago..

Ma revansez la tema 5 :D 


*/

