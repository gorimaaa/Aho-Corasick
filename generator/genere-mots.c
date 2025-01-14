#include "genere-mots.h"
#include <sys/stat.h>
#include <sys/types.h>

/* 
./genere-mots NUMBER_OF_WORDS MIN_LENGTH MAX_LENGTH ALPHABET_SIZE
*/


int random_nmb_min_max(int min, int max);

int main(int argc, char *argv[]){
    if (argc != 5) {
        fprintf(stderr, "Usage :\n");
        fprintf(stderr, "Word generator :   ./genere-mots NUMBER_OF_WORDS MIN_LENGTH MAX_LENGTH ALPHABET_SIZE\n");
        return EXIT_FAILURE;
    }

    int num_words = atoi(argv[1]);
    int min_word_length = atoi(argv[2]);
    int max_word_length = atoi(argv[3]);
    int alphabet_size = atoi(argv[4]);

    if (num_words <= 0 || min_word_length <= 0 ||  max_word_length <= 0 || alphabet_size <= 0) {
        fprintf(stderr, "Erreur : MIN_LENGTH, MAX_LENGTH, WORD_LENGTH et ALPHABET_SIZE doivent être des entiers strictement positifs.\n");
        return EXIT_FAILURE;
    }

    generate_words(num_words, min_word_length, max_word_length, alphabet_size);


    return EXIT_SUCCESS;
}

// Fonction pour générer des mots pseudo-aléatoires
void generate_words(int num_words, int min_word_length, int max_word_length, int alphabet_size) {
    unsigned char min_ascii = 0;
    if(alphabet_size > 27){
        min_ascii = 33;
    }else{
        min_ascii = 97;
    }

    srand((unsigned int)time(NULL)); // Initialisation de la graine aléatoire
    
    for (int i = 0; i < num_words; i++) {
        int random_length = random_nmb_min_max(min_word_length, max_word_length);   
        char *str = malloc((size_t)random_length + 1);
        if(str == NULL){
            fprintf(stderr, "Erreur dans l'allocation de str\n");
        }

        for (int j = 0; j < random_length; j++) {
            char random_char = (char)((int)min_ascii + (rand() % alphabet_size)); // Génération d'un caractère aléatoire
            str[j] = random_char;
        }
        str[random_length] = '\0';
        printf("%s\n", str);
        free(str);
    }
}


int random_nmb_min_max(int min, int max){
    return (rand() % (max - min + 1) + min);
}
