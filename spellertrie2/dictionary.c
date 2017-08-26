/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

#define ASCLO 97
#define APOS 39
#define ASCUP 65

// Node Definition
typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;

// Pointer to top of the trie
node *root = NULL;

// Initializing count for size
unsigned int count = 0;
unsigned int index = 0;

// Unload function declaration
void unload2(node *cursor);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // Setting cursor as root for every word
    node *cursor = root;
    
    // For each letter in input word
    for (int j = 0; word[j] != 0; j++)
    {
        // If character is an alphabet
        if(isalpha(word[j]))
        {
            // If character is uppercase
            if(isupper(word[j]))
            {
                if (cursor->children[word[j] - ASCUP] == NULL)
                    return false;
                else
                    cursor = cursor->children[word[j] - ASCUP];
            }
            
            // If character is lowercase
            else
            {
                if (cursor->children[word[j] - ASCLO] == NULL)
                    return false;
                else
                    cursor = cursor->children[word[j] - ASCLO];
            }
        }
        
        // If character is an apostrophe
        else
        {
            if (cursor->children[26] == NULL)
                return false;
            else
                cursor = cursor->children[26];
        }
        
        // If at end of input word
        if (word[j + 1] == 0)
        {
            if (cursor->is_word)
                return true;
        }
    }
    
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        unload();
        return false;
    }
    
    // Initialising the variable word
    char word[LENGTH + 1] = {0};
    
    // Allocating the root node of the trie
    root = calloc(1, sizeof(node) * 367090);
    if (root == NULL)
    {
        unload();
        return false;
    }
    index++;
    
    // Scanning dictionary word by word until the end of file
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Initializing a traversal pointer to the root each word
        node *trav = root;
        
        // Iterating through the word
        for (int i = 0; word[i] != 0; i++)
        {
            // If alphabet
            if (isalpha(word[i]))
            {
                // If children node is NULL, allocate new node and point trav to it. Else continue to next node.
                if (trav->children[(word[i] - ASCLO)] == NULL)
                {
                    node *new_node = root + index;
                    trav->children[(word[i] - ASCLO)] = new_node;
                    trav = new_node;
                    index++;
                }
                else
                    trav = trav->children[(word[i] - ASCLO)];
            }   
            
            // If apostrophe
            else
            {
                // If last children node is NULL, allocate new node and point trav to it. Else continue to next node.
                if (trav->children[26] == NULL)
                {
                    node *new_node = root + index;
                    trav->children [26] = new_node;
                    trav = new_node;
                    index++;
                }
                else
                    trav = trav->children[26];
            }
            
            // Setting flag to true if at the end of the word
            if (word[i + 1] == 0)
            {
                trav->is_word = true;
            }
        }
        
        // Incrementing count
        count++;
    }
    
    fclose (dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    free(root);
    
    return true;
}

