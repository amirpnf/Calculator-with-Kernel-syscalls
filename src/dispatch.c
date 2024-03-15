#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include "try.h"

    // CONVENTION 
    // ADD index = 0
    // SUB index = 1
    // MULT index = 2
    // DIV index = 3

#define BUF_SIZE 64
#define NBR_PROCESSES 4

typedef struct {
    int in[2];
    int out[2];
    char* name;
} Process;


static void start(Process* tab, int index) {
    try(pipe(tab[index].in));
    try(pipe(tab[index].out));
    switch(try(fork())) {
        
        case 0 : // CHILD
            try(dup2(tab[index].in[0], STDIN_FILENO));
            try(dup2(tab[index].out[1], STDOUT_FILENO));

            try(close(tab[index].in[0]));
            try(close(tab[index].out[1]));

            try(close(tab[index].in[1]));
            try(close(tab[index].out[0]));

            for(int j = 0; j < index; j++) {
                //try(close(tab[j].in[0]));
                try(close(tab[j].in[1]));
                try(close(tab[j].out[0]));
                //try(close(tab[j].out[1]));
            }

            try(execlp(tab[index].name, tab[index].name, (char*) NULL));
            break;
        default: // PARENT
            try(close(tab[index].in[0]));
            try(close(tab[index].out[1]));
            break;    

    }
}


static void rec_commands(char* buf, char* result, Process* processes) {
    int index;
    int op1, op2;
    char command[BUF_SIZE];

    while(!feof(stdin)) {

        fgets(buf, BUF_SIZE, stdin);
        if(!strncmp(buf, "add", 3)) {
            index = 0;
            sscanf(buf, "add %d %d", &op1, &op2); 
            //printf("%d %d \n", op1, op2);
        }
        if(!strncmp(buf, "sub", 3)) {
            index = 1;
            sscanf(buf, "sub %d %d", &op1, &op2);
            //printf("%d %d \n", op1, op2);
        }
        if(!strncmp(buf, "div", 3)) {
            index = 3;
            sscanf(buf, "div %d %d", &op1, &op2);
            //printf("%d %d \n", op1, op2);
        }
        if(!strncmp(buf, "mult", 4)) {
            index = 2;
            sscanf(buf, "mult %d %d", &op1, &op2);
            //printf("%d %d \n", op1, op2);
        }

        sprintf(command, "%d %d", op1, op2);
        
        try(write(processes[index].in[1], command, BUF_SIZE));
        try(read(processes[index].out[0], result, BUF_SIZE));
        printf("%s\n", result);
        memset(result, 0, BUF_SIZE);
    }    
}


int main(int argc, char* argv[]) {

    char buf[BUF_SIZE];
    char result[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    memset(result, 0, BUF_SIZE);

    Process processes[NBR_PROCESSES] = {
        {.name =  "./build/add"}, {.name =  "./build/sub"},
        {.name = "./build/mult"}, {.name = "./build/div"}
    };
    for(int i = 0; i < NBR_PROCESSES; i++) { // start 4 processes
        start(processes, i); 
    }
    
    rec_commands(buf, result, processes);

    for(int i = 0; i < NBR_PROCESSES; i++) {
        try(close(processes[i].in[1]));
        try(close(processes[i].out[0]));
    }
    return 0;
}