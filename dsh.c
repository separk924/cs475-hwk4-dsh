/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>
#include "builtins.h"

// TODO: Your function definitions (declarations in dsh.h)
#define HISTORY_LEN 100
#define MAX_IN 256
char *c = " ";
char *colon = ":";
char *path = "PATH";
char *home = "HOME";
int i = 0; // counter for history
char history[HISTORY_LEN][HISTORY_LEN] = {""};

/**
 * This function parses through the input and opens the file if it exists
 * 
 * @param str The file path
 */
void parse(char *str){
    char *token = strtok(str,c); 
    token[strlen(token) - 1] = '\0';
    hist(token);
    FILE *file;
    if(token[0] == '/'){ // Option 1
        if(access(str, F_OK || X_OK) != -1){ // if file exists
            if(fork() != 0){ // parent 
		        wait(NULL);
            }else{ // child process
                file = fopen(token, "r");
                int status = execv(token,file);
                if (status != 0){
                    printf("Error\n");
                    exit(-1);
                }
            }
	    }else{
            if(token[strlen(token)-1] == '&'){
                if(fork() != 0){ // parent process
                    
                }else{ // child process
                    file = fopen(token, "r");
                    int status = execv(token,file);
                if (status != 0){
                    printf("Error\n");
                    exit(-1);
                }
            }
            }else{
                printf("'%s' does not exist\n", token);
            }
        }
    }else{ // Option 2
        if(strcmp(token, "exit") == 0){
            exit(-1);
        }else if(strcmp(token,"pwd") == 0){
            pwd();
        }else if(strcmp(token, "history") == 0){
            printHistory();
        }else if(strcmp(token, "cd") == 0){
            cd(str);
        }else{
        char cwd[MAX_IN];
        getcwd(cwd, sizeof(cwd));
        strcat(cwd, "/");
        strcat(cwd, token); // concat the cwd & user input
        if(access(cwd, F_OK) != -1){ // if file exists
            if(fork() != 0){ // parent process
		        wait(NULL);
            }else{ // child process
                file = fopen(token, "r");
                int status = execv(token,file);
                if (status != 0){
                    printf("Error");
                    exit(-1);
                }
            }
	    }else{
            char *value;
            value = getenv(path);
            printf("%s\n", value);
            char *tok = strtok(value,colon);
            char *temp;
            while(tok != NULL){
                temp = tok;
                temp[strlen(temp) - 1] = '\0';
                strcat(temp, "/");
                strcat(temp, token); // concat the cwd & user input
                if(access(temp, F_OK) != -1){ // if file exists
                    if(fork() != 0){ // parent process
                        wait(NULL);
                    }else{ // child process
                        file = fopen(temp, "r");
                        int status = execv(temp,file);
                        if (status != 0){
                            printf("Error\n");
                            exit(-1);
                        }
                    }
                }
                tok = strtok(NULL,colon);
            }
        }
    }
    }
    token = strtok(NULL,c);
}

/**
 * This function prints the current working directory
 */
void pwd(){
    char cwd[MAX_IN];
    getcwd(cwd, sizeof(cwd));
    printf(" %s\n", cwd);
}

/**
 * @brief This function changes the current working directory to the inputted path
 * 
 * @param str The inputted path
 */
void cd(char *str){
    if(strcmp(str, "cd") == 0){
        char *value;
        value = getenv(home);
        printf("%s\n", value);
        chdir(value);
    }else{
        str = strtok(NULL, " ");
        chdir(str);
    }
}

/**
 * @brief This function adds the most recent commands to an array
 * 
 * @param str The command to add onto the history list
 */
void hist(char *str){
    if(i>=HISTORY_LEN){
        i=0;
    }else{
        strcpy(history[i], str);
        i++;
    }
}

/**
 * @brief Prints the history of commands inputted by the user
 */
void printHistory(){
    for(int j=0; j<=i;j++){
        printf("%s\n", history[j]);
    }
}