#include "ac-matrice.h"

int AC(unsigned char **mots, size_t line_count, char *texte, size_t taille_texte)
{
    int occurence = 0;
    int e = 0;
    Trie trie = PRE_AC(mots, line_count);
    for (size_t j = 0; j < taille_texte; j++)
    {
        while (trie->transition[e][(unsigned char)texte[j]] == -1)
        {
            e = trie->sup[e];
        }
        e = trie->transition[e][(unsigned char)texte[j]];
        if (trie->finite[e] != NULL)
        {
            occurence += trie->finite[e][0];
        }
    }
    //afficherTransitionsAvecFinite(trie);
    freeTrie(trie);
    return occurence;
}

Trie PRE_AC(unsigned char **mots, size_t line_count)
{
    Trie trie = createTrie(TRIE_SIZE_MAX);
    for (size_t i = 0; i < line_count; i++)
    {
        insertInTrie(mots[i], trie, i);
    }
    // afficherTransitionsAvecFinite(trie);
    for (int i = 0; i < UCHAR_MAX; i++)
    {
        if (trie->transition[0][i] == -1)
        {
            trie->transition[0][i] = 0;
        }
    }
    // afficherTransitionsAvecFinite(trie);
    completTrie(trie);
    //afficherTransitionsAvecFinite(trie);
    return trie;
}

Trie createTrie(int maxNode)
{
    Trie table_transition = (Trie)malloc(sizeof(struct _trie));

    if (table_transition == NULL)
    {
        printf("Erreur d'allocation\n");
        exit(EXIT_FAILURE);
    }

    table_transition->maxNode = maxNode;
    table_transition->nextNode = 1;
    table_transition->transition = (int **)malloc((long unsigned int)maxNode * sizeof(int *));
    if (table_transition->transition == NULL)
    {
        printf("Erreur alloc transitio\n");
        free(table_transition);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < maxNode; i++)
    {
        table_transition->transition[i] = (int *)malloc(UCHAR_MAX * sizeof(int));
        if (table_transition->transition[i] == NULL)
        {
            printf("PB allocation pour transition[%d]", i);
            for (int j = 0; j < i; j++)
            {
                free(table_transition->transition[j]);
            }
            free(table_transition->transition);
            free(table_transition);
            exit(EXIT_FAILURE);
        }
        // Initialiser chaque case à -1
        for (int k = 0; k < UCHAR_MAX; k++)
        {
            table_transition->transition[i][k] = -1;
        }
    }
    table_transition->finite = (int **)malloc((long unsigned int)maxNode * sizeof(int *));
    if (table_transition->finite == NULL)
    {
        printf("Erreur alloc finite");
        for (int i = 0; i < maxNode; i++)
        {
            free(table_transition->transition[i]);
        }
        free(table_transition->transition);
        free(table_transition);
        exit(EXIT_FAILURE);
    }

    table_transition->sup = (int *)malloc((long unsigned int)maxNode * sizeof(int));
    if (table_transition->sup == NULL)
    {
        printf("Erreur d'allocation pour sup\n");
        for (int i = 0; i < maxNode; i++)
        {
            free(table_transition->transition[i]);
        }
        free(table_transition->transition);
        free(table_transition->finite);
        free(table_transition);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < maxNode; i++)
    {
        table_transition->sup[i] = -1; // Valeur par défaut
        table_transition->finite[i] = NULL;
    }

    return table_transition;
}

void insertInTrie(unsigned char *mot, Trie table_transition, size_t line)
{
    int saveNode = 0;
    unsigned char actuel;
    int i = 0;
    while (mot[i] != '\0')
    {
        actuel = mot[i];
        if (table_transition->transition[saveNode][actuel] != -1)
        {
            saveNode = table_transition->transition[saveNode][actuel];
        }
        else
        {
            if (table_transition->nextNode >= table_transition->maxNode)
            {
                printf("Trop de noeud demandé ! Tache non réalisé\n\n");
                exit(EXIT_FAILURE);
            }
            table_transition->transition[saveNode][actuel] = table_transition->nextNode;
            saveNode = table_transition->nextNode;
            table_transition->nextNode += 1;
        }
        i++;
    }
    if(table_transition->finite[saveNode] == NULL){
    table_transition->finite[saveNode] = creerTableau();
    table_transition->finite[saveNode][0] = 1;
    table_transition->finite[saveNode][1] = (int)line;
    }else{
        int j = table_transition->finite[saveNode][0];
        table_transition->finite[saveNode] = realloc(table_transition->finite[saveNode], sizeof(int) * (long unsigned int)(j + 1));
        (table_transition->finite[saveNode])[j] = (int)line;
    }

}

void completTrie(Trie trie)
{
    Queue *queue = createQueue();

    // Ajouter les transitions initiales et définir les suppléances
    for (int r = 0; r < trie->maxNode; r++)
    {
        for (int a = 0; a < UCHAR_MAX; a++)
        {
            int p = trie->transition[r][a];
            if (p != -1 && p != r)
            {
                enqueue(queue, p);
                trie->sup[p] = r;
            }
        }
    }

    // Propagation des suppléances
    while (!isEmpty(queue))
    {
        int r = dequeue(queue);
        for (int a = 0; a < UCHAR_MAX; a++)
        {
            int p = trie->transition[r][a];
            if (p != -1 && p != r)
            {
                enqueue(queue, p);
                int s = trie->sup[r];
                while (trie->transition[s][a] == -1)
                {
                    s = trie->sup[s];
                }
                trie->sup[p] = trie->transition[s][a];
                // fonction de réallocation si nécessaire
                transfertValeurs(&(trie->finite[p]), trie->finite[trie->sup[p]]);
            }
        }
    }
    freeQueue(queue);
}





void freeTrie(Trie table_transition)
{
    free(table_transition->sup);
    for (int i = 0; i < table_transition->maxNode; i++)
    {
        free(table_transition->transition[i]);
        if(table_transition->finite[i] != NULL){
            free(table_transition->finite[i]);
        }
        
        // free(&table_transition->sup[i]);
    }
    free(table_transition->finite);
    free(table_transition->transition);
    free(table_transition);
}

void afficherTransitionsAvecFinite(Trie table_transition)
{
    if (table_transition == NULL)
    {
        printf("Trie non initialisé.\n");
        return;
    }

    printf("Affichage des transitions et états finaux du Trie :\n");
    for (int i = 0; i < table_transition->maxNode; i++)
    {
        // Vérifiez si le nœud est un état final
        if (table_transition->finite[i] != NULL)
        {
            printf("Nœud %d (État final); sup = %d:\n", i, table_transition->sup[i]);
        }
        else
        {
            printf("Nœud %d ; sup = %d:\n", i, table_transition->sup[i]);
        }

        int hasTransitions = 0; // Pour vérifier si un nœud a des transitions
        for (int j = 0; j < UCHAR_MAX; j++)
        {
            if (table_transition->transition[i][j] != -1)
            {
                hasTransitions = 1;
                printf("  '%c' -> %d\n", j, table_transition->transition[i][j]);
            }
        }
        if (!hasTransitions)
        {
            printf("  Aucune transition valide\n");
        }
    }
}



// Fonction pour créer un tableau avec taille initiale 4
int *creerTableau()
{
    int tailleInitiale = 2;
    int *tableau = (int *)malloc((long unsigned int) tailleInitiale * sizeof(int));
    if (tableau == NULL)
    {
        printf("Erreur d'allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }
    return tableau;
}

int estValeurPresente(int* tableau, int taille, int valeur) {
    for (int i = 1; i <= taille; ++i) {
        if (tableau[i] == valeur) {
            return 1;
        }
    }
    return 0;
}

void transfertValeurs(int** tableau1, int* tableau2) {
    if (*tableau1 == NULL && tableau2 == NULL) {
        return;
    }
    if (tableau2 == NULL) {
        //printf("Tableau2 non initialisé, aucun transfert effectué.\n");
        return;
    }
    if (*tableau1 == NULL) {
        *tableau1 = malloc(sizeof(int) * (long unsigned int)(tableau2[0] + 1));
        (*tableau1)[0] = 0;
    }

    int taille1 = (*tableau1)[0];
    int taille2 = tableau2[0];

    for (int i = 1; i <= taille2; ++i) {
        if (!estValeurPresente(*tableau1, taille1, tableau2[i])) {
            taille1++;
            *tableau1 = realloc(*tableau1, sizeof(int) * (long unsigned int)(taille1 + 1));
            (*tableau1)[taille1] = tableau2[i];
        }
    }

    (*tableau1)[0] = taille1;
}