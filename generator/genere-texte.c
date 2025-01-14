#include "genere-texte.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <taille_texte> <taille_alphabet>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    char *endptr;
    long int taille_texte = strtol(argv[1], &endptr, 10);
    if (errno == ERANGE)
    {
        fprintf(stderr, "Erreur de conversion pour la taille du texte.\n");
        return 1;
    }
    
    int taille_alphabet = (int)strtol(argv[2], &endptr, 10);
    if (errno == ERANGE || taille_alphabet <= 0 || taille_alphabet > 95)
    {
        fprintf(stderr, "Erreur de conversion ou taille d'alphabet invalide.\n");
        return 1;
    }
    generate_random_text(taille_texte, taille_alphabet);

    return EXIT_SUCCESS;
}

void generate_random_text(long int longueur, int taille_alphabet)
{
    int min_ascii = 97;
    if (taille_alphabet > 27)
    {
        min_ascii = 33;
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand((unsigned int)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
    for (long int i = 0; i < longueur; i++)
    {
        char caractere = (char)(min_ascii + rand() % taille_alphabet);
        fprintf(stdout, "%c", caractere);
    }
}