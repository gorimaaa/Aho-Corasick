#include <stdio.h>
#include "ac-hashtable.h"
#include <string.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10
#define LINE_MAX_LENGTH 1000000
// La fonction lit un fichier ligne et renvoie un tableau contenant ces lignes, line_count est mis
// à jour avec le nombre de lignes lues
unsigned char **read_file_lines(const char *filename, size_t *line_count);

// Libère ce qui a été alloué dans read_file_lines
void free_lines(unsigned char **lines, size_t line_count);

// Fonction pour lire une seule ligne d'un fichier et la stocker dans une variable
char *read_single_line(const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s mots.txt texte.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *mots_filename = argv[1];
    const char *texte_filename = argv[2];

    size_t line_count;
    unsigned char **words = read_file_lines(mots_filename, &line_count);
    if (words == NULL) {
        fprintf(stderr, "Erreur dans la lecture des mots\n");
        return EXIT_FAILURE;
    }

    char *texte = read_single_line(texte_filename);
    if (texte == NULL) {
        fprintf(stderr, "Erreur dans la lecture du texte\n");
        free_lines(words, line_count);
        return EXIT_FAILURE;
    }


    int occurences = Aho_Corasick(words, line_count, texte, strlen(texte));
    if(occurences == -1){
        fprintf(stderr, "Une erreur s'est produite dans l'algorithme d'Aho-corasick\n");
    }else{
        printf("%d\n", occurences);
    }

    free_lines(words, line_count);
    free(texte);

    return EXIT_SUCCESS;
}

/**
 * Lit un fichier ligne par ligne et retourne un tableau dynamique
 * contenant les lignes lues.
*/
unsigned char **read_file_lines(const char *filename, size_t *line_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    size_t capacity = INITIAL_CAPACITY; // Capacité initiale pour le tableau de lignes
    size_t count = 0;                  // Nombre de lignes lues
    unsigned char **lines = malloc(capacity * sizeof(unsigned char *));
    if (!lines) {
        perror("Erreur d'allocation mémoire");
        fclose(file);
        return NULL;
    }

    char buffer[LINE_MAX_LENGTH]; // Tampon fixe pour chaque ligne
    while (fgets(buffer, LINE_MAX_LENGTH, file)) {
        // Supprimer le caractère de fin de ligne, s'il existe
        buffer[strcspn(buffer, "\n")] = '\0';

        // Allouer la mémoire pour la ligne lue
        size_t line_length = strlen(buffer);
        unsigned char *line = malloc((line_length + 1) * sizeof(unsigned char));
        if (!line) {
            perror("Erreur d'allocation mémoire pour une ligne");
            // Libérer toutes les lignes déjà allouées
            for (size_t i = 0; i < count; i++) {
                free(lines[i]);
            }
            free(lines);
            fclose(file);
            return NULL;
        }

        // Copier le contenu de la ligne
        strcpy((char *)line, buffer);

        // Ajouter la ligne au tableau
        if (count >= capacity) {
            capacity *= 2; // Doubler la capacité du tableau si nécessaire
            unsigned char **new_lines = realloc(lines, capacity * sizeof(unsigned char *));
            if (!new_lines) {
                perror("Erreur de réallocation mémoire");
                free(line); // Libérer la ligne actuelle
                for (size_t i = 0; i < count; i++) {
                    free(lines[i]);
                }
                free(lines);
                fclose(file);
                return NULL;
            }
            lines = new_lines;
        }

        lines[count++] = line; // Ajouter la ligne au tableau
    }

    fclose(file);
    *line_count = count;
    return lines; // Retourner le tableau de lignes
}

/**
 * Lit une seule ligne depuis un fichier et la retourne.
 */
char *read_single_line(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    // Calcul de la taille du fichier
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    if (file_size <= 0) {
        fprintf(stderr, "Erreur : fichier vide ou inaccessible\n");
        fclose(file);
        return NULL;
    }

    // Allouer un tampon de la taille du fichier
    char *buffer = malloc((size_t)(file_size + 1));
    if (!buffer) {
        perror("Erreur d'allocation mémoire pour le tampon");
        fclose(file);
        return NULL;
    }

    // Lire la ligne
    if (!fgets(buffer, (int)(file_size + 1), file)) {
        perror("Erreur lors de la lecture du fichier");
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);

    // Supprimer le \n final s'il existe
    buffer[strcspn(buffer, "\n")] = '\0';

    return buffer; // La mémoire est déjà allouée pour `line`
}

/**
 * Libère la mémoire allouée pour le tableau de lignes.
 */
void free_lines(unsigned char **lines, size_t line_count)
{
    for (size_t i = 0; i < line_count; i++)
    {
        free(lines[i]);
    }
    free(lines);
}