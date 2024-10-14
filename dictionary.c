#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dictionary.h"
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Keep track of number of words in dictionary
unsigned int word_count = 0;

bool check(const char *word)
{
    // Hash the word to obtain an index
    unsigned int index = hash(word);

    // Access linked list at that index in the hash table
    node *cursor = table[index];

    // Traverse linked list, looking for the word (case-insensitive)
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o dicionário: %s\n", dictionary);
        return false;
    }

    // Read each word from the dictionary file
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for each word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        // Copy the word into the new node
        strcpy(new_node->word, word);

        // Hash the word to obtain an index
        unsigned int index = hash(word);

        // Insert the node into the hash table
        new_node->next = table[index];
        table[index] = new_node;

        // Increment word count
        word_count++;
    }

    // Close the dictionary file
    fclose(file);
    return true;
}

unsigned int hash(const char *word)
{
    // Simple hash function: use the first letter
    return tolower(word[0]) - 'a';
}

unsigned int size(void)
{
    return word_count;
}

bool unload(void)
{
    // Iterate over each bucket in the hash table
    for (unsigned int i = 0; i < N; i++)
    {
        // Free linked list at each bucket
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}

