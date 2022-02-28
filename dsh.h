/*
 * dsh.h
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */

#define MAXBUF 256
#define HISTORY_LEN 100

// TODO: Your function prototypes below

void parse(char *str);
void exit();
void pwd();
void cd(char *str);
void hist(char *str);
void printHistory();