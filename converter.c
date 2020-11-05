/*
 * Name: Jamshed Ashurov
 * PID: A15475198
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "converter.h"

int main(int argc, char **argv) {

	int columncnt = 0; //input column
	int out_columncnt = 0; //output column
	int i = 0; //the incrementor
	int arg_cnvtr = 0; //the variable that converts the column numbers to int
	int opt = 0; //var for parsing 
	int err_opt = 0; //error in parsing 
	int linecnt = 0; //the counter for the lines(records)
	int f_col_flag = 0; //detector if the number of fieds do not match with the specifed columns.
	int *out_line = 0; //output buffer
	int quat_flag = 0; // determines if the comma is in the quatation marks
	char *buf = NULL; //the buffer pointer
	char **in_line; //the input buffer

	size_t bufcap = 0; 

	while ((opt = getopt(argc, argv, "c:")) != -1) {
		switch (opt) {
		case 'c':
			if ((columncnt = (int)strtol(optarg, (char **)NULL, 10)) <= 0) {
				err_opt = 1; 
			}
			break;
		default:
			err_opt = 1;
			break; 
		}
	}
	
	out_columncnt = argc - optind; 
	if ((err_opt == 1) || (out_columncnt <= 0)) {
		print_usage(argv[0]); 
	}
	in_line = (char **)malloc(sizeof(char *) * columncnt);
	out_line = (int *)malloc(sizeof(int) * out_columncnt); 
	while (getline(&buf, &bufcap, stdin) > 0) {
		linecnt++;
		for (i = 0; i <= columncnt; i++) {
			if ((*buf == '\0') && (i == columncnt)) {
				f_col_flag = 0;
				break; 
			}
			else if (((*buf != '\0') && (i == columncnt)) || ((*buf == '\0') && (i != columncnt))) {
				fprintf(stderr, "%s: dropping record #%d\n", argv[0], linecnt); 
				f_col_flag = 1;
				break; 
			}

			*(in_line + i) = buf;
			while (*buf != '\0') {
				if (*buf == '"') {
					if (quat_flag == 0) {
						quat_flag = 1; 
					}
					else {
						quat_flag = 0;
					}
				}
				if (((*buf == ',') && (quat_flag == 0)) || (*buf == '\n')) {
					*buf++ = '\0'; 
					break;
				}
				
				buf++;
			}
		}
		
		if (f_col_flag == 0) {
			for (i = out_columncnt; i > 0; i--) {
				arg_cnvtr = atoi(*(argv + argc - i)); 
				if ((arg_cnvtr <= columncnt) && (arg_cnvtr > 0)) {
					out_line[out_columncnt - i] = arg_cnvtr - 1;
				}
				else {
					print_usage(argv[0]); 
				}	
			}
			for (i = 0; i < out_columncnt; i++) {
				if (i == out_columncnt - 1) {
					printf("%s\n",in_line[out_line[i]]); 
				}
				else {
					printf("%s,",in_line[out_line[i]]);
				}

			}		
		}		
	}
	exit(EXIT_SUCCESS);
}

	void print_usage(char *name) {
		fprintf(stderr, "usage: %s [-c] input_column_count col# [col# ...]\n", name); 
		exit(EXIT_FAILURE); 
	}

