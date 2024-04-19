#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET];
    int count;
};

// Trie structure
struct Trie {
    struct TrieNode *root;
};

// Create a new Trie node
struct TrieNode *createTrieNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node) {
        node->count = 0;
        for (int i = 0; i < ALPHABET; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    // Increment the count of occurrences for the last node
    current->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            // Word does not exist in the Trie
            return 0;
        }
        current = current->children[index];
    }
    // Return the count of occurrences for the last node
    return current->count;
}

// Deallocate the Trie structure
void deallocateTrieHelper(struct TrieNode *node) {
    if (node) {
        for (int i = 0; i < ALPHABET; i++) {
            deallocateTrieHelper(node->children[i]);
        }
        free(node);
    }
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (pTrie) {
        deallocateTrieHelper(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie) {
        pTrie->root = createTrieNode();
    }
    return pTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    int numWords = 0;
    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];
    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }
    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}