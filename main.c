/*
 * main.c
 *
 *  Created on: May 21, 2016
 *      Author: Freeman
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_ALLOC "alloc";

void handle_alloc();
void handle_free();
void handle_view();

int main(int argc, char** argv)
{
	char command[256];

	while (1)
	{
		printf("Enter command: ");
		scanf("%s", command);
		if (strcmp(command, "alloc") == 0)
		{
			printf("Handling alloc...\r\n");
			handle_alloc();
		}
		if (strcmp(command, "free") == 0) {
			printf("Handling free...\r\n");
			handle_free();
		}
		if (strcmp(command, "view") == 0) {
			printf("Handling view...\r\n");
			handle_view();
		}
		if (strcmp(command, "q") == 0) {
			break;
		}
	}
}

void handle_free()
{

}

void handle_alloc()
{

}

void handle_view()
{

}
