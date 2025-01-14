# Répertoires
ac_hashtable_dir = ../ac-hashtable/
file_dir = ../file/
generator_dir = ../generator/
ac_matrice_dir = ../ac-matrice/

# Compilateur et options
CC = gcc
CFLAGS = -std=c18 -g \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2

# Directives pour les fichiers d'en-tête
CFLAGS += -I$(ac_hashtable_dir) -I$(file_dir) -I$(generator_dir) -I$(ac_matrice_dir)

# Fichiers objets et exécutables
hashtable_objects = $(ac_hashtable_dir)ac-hashtable.o
file_object = $(file_dir)file.o
main_object = main_hashtable.o
hashtable_executable = ac-hachage

matrice_objects = $(ac_matrice_dir)ac-matrice.o
main_matrice_object = main_matrice.o
matrice_executable = ac-matrice

genere_mots_object = $(generator_dir)genere-mots.o
genere_mots_executable = genere-mots

genere_texte_object = $(generator_dir)genere-texte.o
genere_texte_executable = genere-texte

# Cibles phony
.PHONY: all clean

# Construction des exécutables
all: $(hashtable_executable)  $(genere_mots_executable) $(genere_texte_executable) $(matrice_executable)

clean:
	rm -f $(hashtable_objects) $(file_object) $(main_object) $(hashtable_executable)
	rm -f $(matrice_objects) $(main_matrice_object) $(matrice_executable)
	rm -f $(genere_mots_object) $(genere_mots_executable)
	rm -f $(genere_texte_object) $(genere_texte_executable)
	rm -rf generated_texts generated_words results

# Règles pour l'exécutable ac-hachage
$(hashtable_executable): $(main_object) $(hashtable_objects) $(file_object)
	$(CC) $(main_object) $(hashtable_objects) $(file_object) -o $@

main_hashtable.o: main_hashtable.c $(ac_hashtable_dir)ac-hashtable.h $(file_dir)file.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ac_hashtable_dir)ac-hashtable.o: $(ac_hashtable_dir)ac-hashtable.c $(ac_hashtable_dir)ac-hashtable.h $(file_dir)file.h
	$(CC) $(CFLAGS) -c $< -o $@

$(file_dir)file.o: $(file_dir)file.c $(file_dir)file.h
	$(CC) $(CFLAGS) -c $< -o $@

# Règles pour l'exécutable ac-matrice
$(matrice_executable): $(main_matrice_object) $(matrice_objects) $(file_object)
	$(CC) $(main_matrice_object) $(matrice_objects) $(file_object) -o $@

main_matrice.o: main_matrice.c $(ac_matrice_dir)ac-matrice.h $(file_dir)file.h
	$(CC) $(CFLAGS) -c $< -o $@

$(ac_matrice_dir)ac-matrice.o: $(ac_matrice_dir)ac-matrice.c $(ac_matrice_dir)ac-matrice.h $(file_dir)file.h
	$(CC) $(CFLAGS) -c $< -o $@

# Règles pour l'exécutable genere-mots
$(genere_mots_executable): $(genere_mots_object)
	$(CC) $(genere_mots_object) -o $@

$(generator_dir)genere-mots.o: $(generator_dir)genere-mots.c $(generator_dir)genere-mots.h
	$(CC) $(CFLAGS) -c $< -o $@

# Règles pour l'exécutable genere-texte
$(genere_texte_executable): $(genere_texte_object)
	$(CC) $(genere_texte_object) -o $@

$(generator_dir)genere-texte.o: $(generator_dir)genere-texte.c $(generator_dir)genere-texte.h
	$(CC) $(CFLAGS) -c $< -o $@
