/*
 * Copyright (c) 2002 
 *      Texas Instruments.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Texas Instruments. 
 * 4. Neither the name of the Company nor of the product may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


/*
 * ledapp.c -- leds manipulation *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include "ledapp.h"

#define MAX_KEYWORDS    5
#define MAX_STATES      MAX_MOD_ID*MAX_MOD_ID


int read_mod(char *line);
int read_state(char *line);
int read_led(char *line);
int read_mode(char *line);
int read_param(char *line);

typedef struct config_kw_handler{
	char *name;
  int (*handler)(char *line);
}config_kw_handler_t;

config_kw_handler_t kwArr[] = {
                              {"module",read_mod},
                              {"state",read_state},
                              {"led",read_led},
                              {"mode",read_mode},
                              {"param",read_param}
};

unsigned int curModule = 0;
char curName[15];

config_elem_t *pTemp;
config_elem_t *ptrArr[MAX_STATES];
int ptrIndex = 0;

char arr[255];

int read_mod(char *line)
{
  /* Remember the new module's name */
  curModule = atoi(line);
  return 1;
}

int read_state(char *line)
{

  /* Malloc memory for new state info */
  pTemp = malloc( sizeof(config_elem_t));
  if(pTemp == NULL)
    return 0;
  /* Remember the pointer */
  ptrArr[ptrIndex++] = (config_elem_t *)pTemp;
  memset(pTemp,0x00,sizeof(config_elem_t));

  /* save the module name and state var */
  pTemp->name = curModule;
  pTemp->state = atoi(line);

  return 1;
}

int read_led(char *line)
{
  /* Save the LED number */
  pTemp->led = atoi(line);
  return 1;
}

int read_mode(char *line)
{
  /* Save the mode */
  pTemp->mode = atoi(line);
  return 1;
}

int read_param(char *line)
{
  pTemp->param = atoi(line);
  return 1;
}


int read_config(char *file)
{
	FILE *in;
	char buffer[80], orig[80], *token, *line;
	int i, j, index;

	
	if (!(in = fopen(file, "r"))) {
    printf("unable to open config file\n");
		return 0;
	}
	index = -1;	
	while (fgets(buffer, 80, in)) {
		if (strchr(buffer, '\n')) *(strchr(buffer, '\n')) = '\0';
		strncpy(orig, buffer, 80);
		if (strchr(buffer, '#')) *(strchr(buffer, '#')) = '\0';
		token = buffer + strspn(buffer, " \t");
		if (*token == '\0') continue;
		line = token + strcspn(token, " \t=");
		if (*line == '\0') continue;
		*line = '\0';
		line++;
		
		/* eat leading whitespace */
		line = line + strspn(line, " \t=");
		/* eat trailing whitespace */
		for (i = strlen(line) ; i > 0 && isspace(line[i-1]); i--);
		line[i] = '\0';
    for(j=0 ; j<MAX_KEYWORDS ; j++)
    {
			if (!strcasecmp(token, kwArr[j].name))
      {
				if (!kwArr[j].handler(line))
        {
          printf("unable to parse %s\n",line);
				  break;
        }
      }
		}
	}
	fclose(in);
	return 1;
}

int main(int argc, char *argv[])
{
	FILE *in;
  unsigned int size;
  int i;
  config_elem_t *ptr;
  int *pBuffer;

  if( argc > 3)
	{
		printf("Ledapp error -- too many parameters \n");
		return -1;
	}
	else
  {
		if( (argc == 3) && ( strcmp( argv[1] , "-c") == 0)  && ( argv[2] != NULL) )
		{
			printf( " argv[1] = %s , argv[2] = %s \n",argv[1],argv[2]);
  		read_config( argv[2] );
		}
		else
		{
			printf( " Standard Configuration File\n");
  		read_config(CONF_FILE);
		}
  }

  /* Allocate a single chunk of memory for the entire table
     to be passed to the module */
  size = sizeof(config_elem_t) * ptrIndex + sizeof(int) + sizeof(int);
  pBuffer = malloc(size);
  memset(pBuffer,0x00,size);
	*pBuffer = 0xFFEEDDCC;
  *(pBuffer + 1) = size - sizeof(int) ;

  ptr = (config_elem_t *)(pBuffer + 2);
  /* Copy from the array of config elements */
  for ( i = 0 ; i < ptrIndex ; i++ )
  {
      *ptr =  *ptrArr[i];
      ptr++;
  }
  printf("Number of State Elements = %d\n",ptrIndex);
  printf("Total Length = %d\n",*(pBuffer + 1));

	if (!(in = fopen(LED_PROC_FILE, "w+"))) {
    printf("unable to open proc file\n");
		return 0;
	}
  i = fwrite(pBuffer, 1, size, in);
  fclose(in);
  free(pBuffer);

  return 0;
}


