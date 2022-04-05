#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <stdio.h>
#include <alpha.hpp>


int main(const int argc, const char *argv[])
{
    if (argc != 3)
    {
        printf("Not enough params in argv\n");
        return 0;
    }

    if (makeAlpha(argv[1], argv[2]))    printf("Error while running\n");

    return 0;
}
