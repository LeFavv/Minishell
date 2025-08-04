#include "minishell.h"

int main()
{
	char s[100];

	printf("%s\n", getcwd(s, 100)); 

    // using the command 
    chdir(".."); 

    // printing current working directory 
    printf("%s\n", getcwd(s, 100)); 
}