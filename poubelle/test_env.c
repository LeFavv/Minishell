#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    
    printf("=== Test getenv() vs env parameter ===\n");
    
    // Test avec getenv()
    char *path_getenv = getenv("PATH");
    printf("getenv(\"PATH\"): %s\n", path_getenv ? path_getenv : "NULL");
    
    // Test avec le paramètre env
    int i = 0;
    char *path_from_env = NULL;
    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
        {
            path_from_env = env[i] + 5;
            break;
        }
        i++;
    }
    printf("from env param: %s\n", path_from_env ? path_from_env : "NULL");
    
    return 0;
}
