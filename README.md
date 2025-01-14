# Aho-Corasick Algorithm Implementation

This repository provides an implementation of the Aho-Corasick string matching algorithm using two data structures:
- **Hash Table**
- **Transition Matrix**

Both implementations are designed to efficiently find all occurrences of multiple patterns within a given text.

## Usage

### Compiling the Project
Navigate to the `main` directory and run:
```bash
make
```
This will generate the following executables:
1. `genere-texte`: Generates random texts.
2. `genere-mots`: Generates random words.
3. `ac-hachage`: Executes the Aho-Corasick algorithm using a hash table.
4. `ac-matrice`: Executes the Aho-Corasick algorithm using a transition matrix.

### Running the Programs
#### Generating Random Text
```bash
./genere-texte <text_length> <alphabet_size> > text.txt
```
#### Generating Random Words
```bash
./genere-mots <word_count> <min_length> <max_length> <alphabet_size> > words.txt
```
#### Running the Aho-Corasick Algorithm
Hash Table Implementation:
```bash
./ac-hachage words.txt text.txt
```

Transition Matrix Implementation:
```bash
./ac-matrice words.txt text.txt
```

### Example Usage
1. Generate a text of length 5,000,000 with an alphabet of size 4:
   ```bash
   ./genere-texte 5000000 4 > text.txt
   ```
2. Generate 100 words with lengths between 5 and 15:
   ```bash
   ./genere-mots 100 5 15 4 > words.txt
   ```
3. Run the Aho-Corasick algorithm:
   ```bash
   ./ac-hachage words.txt text.txt
   ./ac-matrice words.txt text.txt
   ```

## Video Demonstration
[Capture vidéo du 2025-01-14 16-54-26.webm](https://github.com/user-attachments/assets/4931ad88-b1be-4794-8419-c1df5cf741ad)
