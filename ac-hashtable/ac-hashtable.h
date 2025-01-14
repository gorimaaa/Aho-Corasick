// Faire une fonction pour créer X (lis un fichier et produit un tableau de chaine de caractères)

#ifndef HASHTABLE__H
#define HASHTABLE__H

#include <stdlib.h>
#include <stdbool.h>
#include <file.h>

#define CONST_HASHTABLE 50

//- STANDARD -------------------------------------------------------------------

struct _list {
    int startNode, /* etat de depart de la transition */
    targetNode; /* cible de la transition */
    unsigned char letter; /* etiquette de la transition */
    struct _list *next; /* maillon suivant */
};
typedef struct _list *List;

struct _trie {
    int maxNode; /* Nombre maximal de noeuds du trie */
    int nextNode; /* Indice du prochain noeud disponible */
    List *transition; /* listes d’adjacence */
    char ***output; /* tableau des sorties */
    int *output_nb; /* tableau associant chaque état à un nombre de sorties associées */
    int *failureLinks; /* tableau associant à chaque état son lien de suppléance */
};
typedef struct _trie *Trie;

// Permet de créer un Trie d'Aho-Corasick, renvoie NULL en cas d'échec, le pointeur vers le Trie en cas de succès
Trie createTrie(int maxNode);

// Permet d'insérer un mot dans le Trie d'Aho-Corasick,
// on suppose que w se termine par \0, renvoie -1 en cas d'échec 0 en cas de succès
int insertInTrie(Trie trie, unsigned char *w);

// Permet de libérer la mémoire alloué par le Trie;
void freeTrie(Trie trie);

// Applique l'algorithme d'aho-corasick et renvoie le nombre d'occurences des mots words dans le texte en cas de succès, -1 sinon
int Aho_Corasick(unsigned char **words, size_t nb_words, char *text, size_t len_text);



#endif
