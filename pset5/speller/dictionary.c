// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

// Number of words in dictionary
unsigned int size_count = 0;

// Insert a word to hash table, returning true if successful, else false
bool insert(const char *word)
{
    // Get hash value
    int hash_value = hash(word);

    // If this node is NULL, allocate new space
    if (table[hash_value] == NULL)
    {
        table[hash_value] = malloc(sizeof(node));
        if (table[hash_value] == NULL)
        {
            return false;
        }

        table[hash_value] -> next = NULL;
        strcpy(table[hash_value] -> word, word);
    }
    else
    {
        // Locate to last and insert
        node *destination = table[hash_value];
        while (destination -> next != NULL)
        {
            destination = destination -> next;
        }

        // Assign value to node
        destination -> next = malloc(sizeof(node));
        destination = destination -> next;
        destination -> next = NULL;
        strcpy(destination -> word, word);
    }

    // Increment size
    size_count ++;

    return true;
}


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_value = hash(word);

    // If the node is NULL, the word is misspelled
    if (table[hash_value] == NULL)
    {
        return false;
    }

    // Iterate nodes
    for (node *p = table[hash_value]; p != NULL; p = p -> next)
    {
        if (strcasecmp(word, p -> word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    register unsigned int seed = 31; // 31 131 1313 13131 131313 etc..
    register unsigned int hash = 0;

    while (*word)
    {
        hash = hash * seed + tolower(*word++);
    }

    return hash % N;

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open file and check NULL
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        return false;
    }


    // Creat buffer and check NULL
    char *buffer = malloc(sizeof(char) * (LENGTH + 1));
    if (buffer == NULL)
    {
        return false;
    }

    int c = 0;

    // Read into buffer and insert to hash table
    while (fscanf(dict_file, "%s", buffer) != EOF)
    {
        if (!insert(buffer))
        {
            return false;
        }
    }

    free(buffer);
    fclose(dict_file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return size_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i ++)
    {
        if (table[i] != NULL)
        {
            // Free list
            for (node *head = table[i]; head != NULL;)
            {
                node *n = head -> next;
                free(head);
                head = n;
            }
        }
    }
    return true;
}

