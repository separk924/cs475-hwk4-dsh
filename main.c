/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"

#define HISTORY_LEN 100

int main(int argc, char **argv)
{
	char cmdline[MAXBUF]; // stores user input from commmand line
	FILE *motd;
	if(access("/home/ubuntu/hw4/cs475-hwk4-dsh/.dsh_motd", F_OK) != -1){ // if file exists
		motd = fopen(".dsh_motd", "r");
	}
	char c = fgetc(motd);
	while(c != EOF){ // print out contents of .dsh_motd
		printf("%c", c);
		c = fgetc(motd);
	}
	fclose(motd);
	printf("\n\n");

	while(cmdline != "exit"){
		printf("dsh> ");
		fgets(cmdline,MAXBUF,stdin);
		parse(cmdline);
	}
	
	return 0;
}
