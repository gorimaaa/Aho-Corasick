#ifndef AC_MATRICE_H
#define AC_MATRICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "file.h"

// Constantes
#define TRIE_SIZE_MAX 10000
#define TAILLE_MAX_MOTS 7

// Structure de données
struct _trie {
    int maxNode;      // Nombre maximal de nœuds du Trie
    int nextNode;     // Indice du prochain nœud disponible
    int **transition; // Matrice de transition
    int **finite;     // États terminaux (tableaux dynamiques)
    int *sup;         // Tableau de suppléance
};

typedef struct _trie *Trie;

// Fonctions principales
/**
 * Crée un Trie avec un nombre maximal de nœuds spécifié.
 */
Trie createTrie(int maxNode);

/**
 * Insère un mot dans le Trie.
 */
void insertInTrie(unsigned char *mot, Trie table_transition, size_t line);

/**
 * Complète le Trie en ajoutant les suppléances nécessaires.
 */
void completTrie(Trie trie);

/**
 * Libère la mémoire associée au Trie.
 */
void freeTrie(Trie table_transition);

/**
 * Affiche les transitions et les états finaux du Trie.
 */
void afficherTransitionsAvecFinite(Trie table_transition);

/**
 * Construit un Trie à partir d'une liste de mots.
 */
Trie PRE_AC(unsigned char **mots, size_t line_count);

/**
 * Recherche les occurrences des motifs dans un texte en utilisant l'algorithme d'Aho-Corasick.
 */
int AC(unsigned char **mots, size_t line_count, char *texte, size_t taille_texte);

// Fonctions utilitaires
/**
 * Crée un tableau dynamique initialisé pour stocker des états ou transitions.
 */
int *creerTableau();


/**
 * Transfère les valeurs d'un tableau vers un autre.
 */
void transfertValeurs(int **tableau1, int *tableau2);

#endif // AC_MATRICE_H
