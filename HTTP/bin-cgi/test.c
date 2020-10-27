#include <stdio.h>

int main(int argc, char **argv, char **env)
{
    int     i = 0;

    printf("Content-Type: text/html\n\n");
    while (env[i])
        printf("%s\n", env[i++]);
    return (0);
}
