// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// total words loaded in dictionary
unsigned int total_loaded_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{

    char *tmp = calloc(LENGTH + 1, sizeof(char));
    if (tmp == NULL)
    {
        return false;
    }

    for (int i = 0; word[i]; i++)
    {
        tmp[i] = tolower(word[i]);
    }
    // find index
    unsigned int index = hash(tmp) % N;
    free(tmp);

    // iterate linked list in index
    node *n = table[index]->next;
    // till end of list
    while (n != NULL)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int sum = 0;
    // takes into account char value and order in word
    for (int i = 0; word[i]; i++)
    {
        sum += (unsigned int)(i + 1) * (unsigned int)word[i];
    }
    return sum;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // init table
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        if (table[i] == NULL)
        {
            return false;
        }
        table[i]->next = NULL;
    }

    // open file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // create temporary use variables;
    char word[LENGTH + 1];

    // read words until end of file
    while (fscanf(file, "%s", word) != EOF)
    {
        // create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }

        // store newfound word in node
        strcpy(n->word, word);
        n->next = NULL;

        // go to head of table[index], point to whatever it points to and then make head point to us
        unsigned int index = hash(word) % N;
        n->next = table[index]->next;
        table[index]->next = n;
        total_loaded_words++;
    }
    fclose(file);

    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_loaded_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *tmp;
    node *cursor;

    for (int i = 0; i < N; i++)
    {
        tmp = table[i]->next;
        cursor = table[i]->next;

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
        free(table[i]);

    }
    return true;
}
