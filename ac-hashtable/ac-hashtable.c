#include <stdlib.h>
#include <stdio.h>
#include "ac-hashtable.h"
#include <limits.h>
#include <string.h>

#define ASCII_NUMBER 256





// Fonction de prétaitement d'aho-corasick, elle crée le trie des mots words et ajoute les liens de suppléances, renvoie le 
// trie en cas de succès, NULL en cas d'échec
Trie pre_ac(unsigned char **words, size_t nb_words);

// Ajoute word dans le tableau associé à l'état state, créer un tableau dans output si nécessaire
// renvoie 0 en cas de succès et -1 en cas d'échec
int insertInOutput(Trie trie, unsigned char *word, int state);

// Ajoute les transitions vers 0 des caractères de la table ascii non définie dans le trie à partir de 0
int addTransitionsTo0(Trie trie);

// Permet d'ajouter les liens de suppléances en complétant le tableau failureLinks, renvoie 0 en cas de succès -1 en cas d'échec
int addFailureLinks(Trie trie);

// Cherche s'il existe une transition depuis state avec letter, renvoie l'état cible s'il existe
// renvoie -1 sinon
int nextTransition(Trie t, int state, unsigned char letter);

// Fonction de hachage 
unsigned int hash_function(int etat, const unsigned char *valeur, unsigned int maxnode);

// Affiche les sorties associées à chaque état (non vide)
void printOutputs(Trie trie);

// Affiche les liens de suppléances associés à chaque état (différent de 0)
void printFailureLinks(Trie trie);


Trie createTrie(int maxNode){
  if(maxNode <= 0){
    fprintf(stderr, "maxNode invalide");
    return NULL;
  }
  Trie q = malloc(sizeof(*q));

  if(q == NULL){
    fprintf(stderr, "Alloc error");
    return NULL;
  }

  q->maxNode = maxNode;
  q->nextNode = 1;

  List *transition = malloc(sizeof(*transition) * ((long unsigned int)maxNode * CONST_HASHTABLE));
  if(transition == NULL){
    fprintf(stderr, "Alloc error");
    free(q);
    return NULL;
  }
  for(int i=0; i < maxNode * CONST_HASHTABLE; i++){
    transition[i] = NULL;
  }
  q->transition = transition;
  
  
  char ***output_tab = malloc(sizeof(*output_tab) * (long unsigned int)maxNode);
  if(output_tab == NULL){
    fprintf(stderr, "Error alloc output_tab");
    free(q->transition);
    free(q);
    return NULL;
  }
  for(int i=0; i < maxNode; i++){
    output_tab[i] = NULL;
  }
  q->output = output_tab;

  int *output_nb_tab = malloc(sizeof(*output_nb_tab) * (long unsigned int)maxNode);
  if(output_nb_tab == NULL){
    fprintf(stderr, "Error alloc output_nb_tab");
    free(q->transition);
    free(q->output);
    free(q);
    return NULL;
  }
  for(int i=0; i < maxNode; i++){
    output_nb_tab[i] = 0;
  }
  q->output_nb = output_nb_tab;

  int *failure_links_tab = malloc(sizeof(*failure_links_tab) * (long unsigned int)maxNode);
  if(failure_links_tab == NULL){
    fprintf(stderr, "Error alloc failure_links_tab");
    free(q->transition);
    free(q->output);
    free(q->output_nb);
    free(q);
    return NULL;
  }
  for(int i=0; i < maxNode; i++){
    failure_links_tab[i] = 0;
  }
  q->failureLinks = failure_links_tab;

  return q;
  
}


void freeTrie(Trie trie) {
    if (trie == NULL) return;

    // Libérer les transitions
    for (int i = 0; i < trie->maxNode * CONST_HASHTABLE; i++) {
        List current = trie->transition[i];
        while (current != NULL) {
            List temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(trie->transition);

    // Libérer les sorties
    for (int i = 0; i < trie->maxNode; i++) {
        if (trie->output[i] != NULL) {
            for (int j = 0; j < trie->output_nb[i]; j++) {
                free(trie->output[i][j]);
            }
            free(trie->output[i]);
        }
    }
    free(trie->output);

    // Libérer les autres tableaux
    free(trie->output_nb);
    free(trie->failureLinks);

    // Libérer la structure principale
    free(trie);
}



int insertInTrie(Trie trie, unsigned char *w){
  size_t index_word = 0;
  int currentState = 0;
  size_t len_word = strlen((const char *)w);

  // Soit p un préfixe de w, on vérifie si un préfixe p existe afin de
  // démarrer l'ajout à la fin du préfixe (factorisation)
  for(int index=0; index < trie->nextNode && index_word < len_word; index++){
    int resultState = nextTransition(trie, currentState, w[index_word]); 
    if(resultState != -1){
      currentState = resultState;
      index_word++;
    }else{
      break;
    }    
  }
  // currentState = Noeud de départ (trouvé précédemment) ou 0 si aucun préfixe trouvé
  // index_word = Indice de strlen(p) + 1
  while(index_word < strlen((const char *)w)){
    size_t hash = (size_t)hash_function(currentState, &w[index_word], (unsigned int)trie->maxNode);
    List q = trie->transition[hash];

    List c = malloc(sizeof(*c));
    if(c == NULL){
      fprintf(stderr, "Erreur d'allocation");
      return -1;
    }

    if(q == NULL){
      trie->transition[hash] = c;
    }else{
      // On arrive à l'endroit de création de cellule s'il en existe déjà
      while(q->next != NULL){
        q = q->next;
      }
      q->next = c;
    }

    c->startNode = currentState;
    c->targetNode = trie->nextNode;
    trie->nextNode+=1;
    c->letter = w[index_word];
    c->next = NULL;

    currentState = c->targetNode;
    (index_word)++;

  }
  if(insertInOutput(trie, w, currentState) == -1){
    fprintf(stderr, "Erreur ajout sortie");
    return -1;
  }
  return 0;
}


int Aho_Corasick(unsigned char **words, size_t nb_words, char *text, size_t len_text){
    Trie trie = pre_ac(words, nb_words);
    if(trie == NULL){
        return -1;
    }
    int occurences = 0;
    int currentState = 0;
    int tempState = 0;
    for(int index_text=0; (size_t)index_text < len_text; index_text++){
      unsigned char letter = (unsigned char)text[index_text];
      while(true){
        tempState = nextTransition(trie, currentState, letter);
        if(tempState == -1){
          currentState = trie->failureLinks[currentState];
        }else{
          currentState = tempState;
          break;
        }
      }
      if(trie->output[currentState] != NULL){
        occurences+=trie->output_nb[currentState];
      }
    }

    freeTrie(trie);
    return occurences;
    
}


Trie pre_ac(unsigned char **words, size_t nb_words){
    size_t maxNode = 1;
    for(size_t index=0; index < nb_words; index++){
        maxNode+=strlen((char *)words[index]);
    }
    Trie trie = createTrie((int)maxNode);
    for(size_t j=0; j < nb_words; j++){
        insertInTrie(trie, words[j]);
    }

    if(addTransitionsTo0(trie) == -1){
        return NULL;
    };

    if(addFailureLinks(trie) == -1){
        return NULL;
    };
    return trie;
}

// TOOLS


int nextTransition(Trie t, int state, unsigned char letter){
  size_t hash = (size_t)hash_function(state, &letter, (unsigned int)t->maxNode);
  List q = t->transition[hash];
  while(q != NULL){
    if(letter == q->letter && q->startNode == state){
      return q->targetNode;
    }else{
      q = q->next;
    }
  }
  return -1;
}

unsigned int hash_function(int etat, const unsigned char *valeur, unsigned int maxnode) {
    unsigned int hash = (unsigned int)etat;
    // Ne hacher qu'un seul caractère (la lettre à l'index *valeur)
    hash = (hash * 31) + *valeur;  // 31 est un nombre premier
    
    // Appliquer modulo pour obtenir une valeur dans la taille de la table de hachage
    unsigned int taille_hashtable = maxnode * CONST_HASHTABLE;
    return hash % taille_hashtable;
}


int insertInOutput(Trie trie, unsigned char *word, int state){
    int index = 0;
    bool wordExist = false;

    size_t length = strlen((char *)word) + 1;
    char *copy = malloc(length);
    if (copy == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire");
        return -1;
    }
    strcpy(copy, (char *)word);
 
    if(trie->output[state] == NULL){
        char **words_tab = malloc(sizeof(char **) * 1);
        if(words_tab == NULL){
            fprintf(stderr, "Erreur allocation");
            return -1;
        }
        trie->output[state] = words_tab;
        trie->output_nb[state]+=1;
        index = 0;
    }else{
        index = trie->output_nb[state];
        for(int i=0; i < index; i++){
          if(strcmp(trie->output[state][i], copy) == 0){
            wordExist = true;
          }
        }
        if(wordExist == false){
          char **new_words_tab = realloc(trie->output[state], (size_t)((int)sizeof(char **) * (trie->output_nb[state] + 1)));
          if(new_words_tab == NULL){
              fprintf(stderr, "Erreur alloc");
              return -1;
          }
          trie->output_nb[state]+=1;
          trie->output[state] = new_words_tab;
        }
    }


    //on vérifie si le mot existe déjà dans la sortie
  
    if(wordExist == false){
      trie->output[state][index] = copy;
    }
    return 0;

}

int addTransitionsTo0(Trie trie){
    for(int ascii_index=0; ascii_index < 256; ascii_index++){
      unsigned char ascii_letter = (unsigned char)ascii_index;
      int resultState = nextTransition(trie, 0, ascii_letter);
      if(resultState == -1){
          size_t hash = (size_t)hash_function(0, &ascii_letter, (unsigned int)trie->maxNode);
          List t = trie->transition[hash];

          List cell = malloc(sizeof(*cell));
          if(cell == NULL){
              fprintf(stderr, "Erreur d'allocation");
              return -1;
          }

          if(t == NULL){
              trie->transition[hash] = cell;
          }else{
              while(t->next != NULL){
                  t = t->next;
              }
              t->next = cell;
          }

          cell->startNode = 0;
          cell->targetNode = 0;
          cell->letter = ascii_letter;
          cell->next = NULL;
      }

    }
    return 0;
}


int addFailureLinks(Trie trie){
    Queue* queue = createQueue();
    for(int ascii_index = 0; ascii_index < ASCII_NUMBER; ascii_index++){
      unsigned char letter = (unsigned char)ascii_index;
      int nextState = nextTransition(trie, 0, letter); 
      if(nextState != -1){
        trie->failureLinks[nextState] = 0;
        if(nextState != 0){
          enqueue(queue, nextState);
        }
      }
    }

    while(!isEmpty(queue)){  
      int currentState = dequeue(queue);
      for(int ascii_index = 0; ascii_index < ASCII_NUMBER; ascii_index++){
        unsigned char letter = (unsigned char)ascii_index;
        int nextState = nextTransition(trie, currentState, letter);
        if(nextState != -1){
          enqueue(queue, nextState);
          int failureLink = trie->failureLinks[currentState];
          int tempState;
          while(true){
            tempState = nextTransition(trie, failureLink, letter);
            if(tempState == -1){
              failureLink = trie->failureLinks[failureLink];
            }else{
              failureLink = tempState;
              break;
            }
          }
          trie->failureLinks[nextState] = failureLink;
          char **outputs = trie->output[trie->failureLinks[nextState]]; 
          if(outputs != NULL){
            for(int index=0; index < trie->output_nb[trie->failureLinks[nextState]]; index++){
                insertInOutput(trie, (unsigned char *)outputs[index], nextState);
            }
          }
        }
      }
    }
    

    return 0;
}

// DEBUG FONCTIONS
void printOutputs(Trie trie){
    for(int state=0; state < trie->maxNode; state++){
        if(trie->output[state] != NULL){
            printf("État : %d  Sortie : ", state);
            for(int word_index=0; word_index < trie->output_nb[state]; word_index++){
                printf("%s, ", trie->output[state][word_index]);
            }
            printf("\n");
        }
    }
}

void printFailureLinks(Trie trie){
    for(int state=0; state < trie->nextNode; state++){
        printf("État : %d  Lien de suppléance : %d\n", state, trie->failureLinks[state]);
    }
}