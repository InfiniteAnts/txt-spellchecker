/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Node definition
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// A hashtable is just an array of nodes
node *hashtable[SIZE] = {NULL};

// Initialising word count
unsigned int count = 0;

// Hash function declaration
unsigned long hash(const char *str);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // Converting the word into lowercase
    char copy[LENGTH + 1] = {0};
    
    for (int l = 0; word[l] != '\0'; l++)
    {
        copy[l] = tolower(word[l]);
    }
    
    // Hashing the lowercase conversion to get the index
    int index = hash(copy);
    
    // Declaring the head and cursor pointers
    node *cursor = hashtable[index];
    
    // Loop till cursor reaches end of linked list
    while (cursor != NULL)
    {
        // Word is checked against all the words in the linked list
        int x = strcasecmp(word, cursor->word);
        
        // If word is found, success
        if (x == 0)
            return true;
            
        // Cursor Pointer goes to the next node in the list    
        cursor = cursor->next;
    }
    
    // Return false if the word is not found in the linked list
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
    
    // Scanning dictionary word by word until the end of file
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Allocating a new node for the current word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        
        // Copying the current word into this node
        strcpy(new_node->word, word);
        
        // index of the hashtable by hashing the word
        int index = hash(new_node->word);
        
        // Next pointer of the node points to head of the linked list in the hash table
        new_node->next = hashtable[index];
        
        // Hash table pointer now points to this new node thus succesfully inserting it to head of the list
        hashtable[index] = new_node;
        
        count++;
    }
    
    fclose(dict);
    return true;
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // returns the count
    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // for loop for traversing down the hashtable
    for (int j = 0; j < SIZE; j++)
    {
        // setting the cursor as the current hashtable index
        node *cursor2 = hashtable[j];
        
        // while loop for traversing from beginning to end of the linked list
        while (cursor2 != NULL)
        {
            // setting a temp pointer equal to the cursor
            node *temp = cursor2;
        
            // Cursor pointer goes to next pointer in the list
            cursor2 = cursor2->next;
            
            free(temp);
        }
    }
    return true;
}

/* 
 *Hash function djb2 (http://www.cse.yorku.ca/~oz/hash.html) 
  written by Daniel J. Bernstein (https://en.wikipedia.org/wiki/Daniel_J._Bernstein)
 */
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash % SIZE);
}
