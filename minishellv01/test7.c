#include "minishell.h"

#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	// char s[100];

	// printf("%s\n", getcwd(s, 100)); 

    // // using the command 
    // chdir(".."); 

    // // printing current working directory 
    // printf("%s\n", getcwd(s, 100)); 

    int stats;
    struct stat buf;  // Structure, pas pointeur

    stats = stat("./test", &buf);  // Passer l'adresse

    if (stats == 0 && S_ISDIR(buf.st_mode))  // Vérifier le succès ET tester st_mode
    {
        printf("isdir\n");
    }
    else if (stats == 0)
    {
        printf("not a directory\n");
    }
    else
    {
        printf("error accessing path\n");
    }
}