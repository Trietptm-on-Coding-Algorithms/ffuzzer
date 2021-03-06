#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
/*
 *	Creates a char with a unique pattern inside
 *	much like Pex in metasploit (www.metasploit.com) :P
 */

unsigned char *
PatternCreate(unsigned int len)
{
	unsigned char x,y,z;
	unsigned char *pattern = NULL;

	unsigned int i = 0;

	if(len > 20280)
	{
		fprintf(stderr, "Lenght is too big ( len > 20280)\n");
		return NULL;
	}

	if((pattern = (unsigned char *) malloc(len+1)) == NULL)
	{
		perror("malloc()");
		return NULL;
	}
	
	pattern[len] = '\0';

	for(x = 'A'; x <= 'Z'; x++)
	{
		for(y = 'a'; y <= 'z'; y++)
		{
			for(z = '0'; z <= '9'; z++)
			{
				pattern[i++] = x;
				if (i == len) return pattern;

                pattern[i++] = y;
                if (i == len) return pattern;

                pattern[i++] = z;
                if (i == len) return pattern;
			}
		}
	}

	return pattern;
}

static unsigned int
calculateOffset(unsigned char *pattern)
{
	unsigned int  offset[3] = {0,0,0};

	offset[0] = ((unsigned int) pattern[0]) - 65;
    offset[1] = ((unsigned int) pattern[1]) - 97;
    offset[2] = ((unsigned int) pattern[2]) - 48;

	return ((offset[0]*26*10 + offset[1]*10 + offset[2])*3);
}

/*
 * Returns the offset inside a buffer generated by PatternCreate()
 * Arguments, the pattern you found using your debugger, ie. EIP = "2Aa3"
 * then this function might return the offset inside your evil buffer
 * or 0 if the pattern has been tampered or doesnt exist.
 */

unsigned int
PatternGetOffset(unsigned char *pattern)
{
	int i;
	unsigned char aux[3];

    /* Search the First Upper Letter */
	for(i = 0; i < 3; i++)
	{
		if(isupper((int) pattern[i]))
		{
			switch(i)
			{
				case 0:
				{
					return calculateOffset(&pattern[i]);
				}
				case 1:
				{
					return calculateOffset(&pattern[i])-1;
				}
				case 2:
                {
					aux[0] = pattern[i];
                    aux[1] = pattern[i+1];
                    aux[2] = pattern[i-1]++;

					return (calculateOffset(&aux[0])+1);
                }
				default:
				{
					fprintf(stderr, "Bad pattern\n");
					return 0;
				}
			}
		}
	}
	
	return 0;
}
