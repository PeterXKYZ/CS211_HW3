#include "libvc.h"

#include <211.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// This definition is private to this file; code in all other files
// can only handle pointers to it: values of type `struct vote_count*`
// (a/k/a `vote_count_t`), not values of type `struct vote_count`. Only
// code in this file sees this definition, so only code in this file can
// work with `struct vote_count`s directly.
struct vote_count
{
    char*  candidate;
    size_t count;
};

// ---------------------helper functions-----------------------

// Returns a pointer to the first element of 'vc' 
// whose 'candidate' is NULL, or NULL if none found
static vote_count_t vc_find_empty(vote_count_t vc) {
    vote_count_t vc_helper = vc; 
    for (int i = 0; i < MAX_CANDIDATES; ++i) {
        if (vc_helper->candidate == NULL) {
            return vc_helper;
        }
        ++vc_helper;
    }
    return NULL; // NULL cannot be found
}

// Returns a pointer to the first element of 'vc'
// whose 'candidate' matches 'name', or NULL if not found
static vote_count_t vc_find_name(vote_count_t vc, const char* name) {    
    vote_count_t vc_helper = vc;
    
    for (int i = 0; i < MAX_CANDIDATES &&
        vc_helper->candidate != NULL; ++i) {
        if (strcmp(vc_helper->candidate, name) == 0) {
        // don't try to use vc_helper->candidate == name
        //  -> only compares the first char of each string
        // make sure vc_helper->candidate is not NULL
        
            return vc_helper;
        }
        ++vc_helper;    
    }
    return NULL; // name cannot be found
}

// Clones a string onto the heap, printing a message to 
// stderr and exiting with code 1 if malloc() fails.
static char* strdup_or_else(const char* src) {
    char* dst = malloc(strlen(src) + 1); 
    // +1 for the null terminator which is not included in strlen()

    if (!dst) { // !NULL = !0 = !false = true
        fprintf(stderr, "Uh oh, can't allocate memory!");
        exit(1);
    }

    return strcpy(dst, src);            
}


// A vote_count_t will be a pointer to a `malloc`ed array of
// MAX_CANDIDATES `struct vote_count`s, with the following invariant:
//
//  - Some prefix of `candidate` fields are distinct, OWNED, `malloc`ed
//  strings, and the remaining `candidate` fields are NULL.
//
//  - When a `candidate` field is NULL the value of the corresponding
//  `count` does not mean anything, but when the `candidate` is non-NULL
//  then the `count` must be a properly initialized count.

/*
 * Q: Where are header comments for the following functions?
 * A: libvc.h
 */

vote_count_t vc_create(void)
{
    vote_count_t vc = malloc(MAX_CANDIDATES * sizeof(struct vote_count));
    // sizeof(vote_count_t) gives the size of a pointer! Not the struct!
    
    if (!vc) { // !NULL = !0 = !false = true
        return NULL;
    }

    vote_count_t vc_helper = vc;

    // this for loop initializes every candidate to be NULL
    // enables vc_destroy() to free all candidate strings w/o issues
    for (int i = 0; i < MAX_CANDIDATES; ++i) {
        vc_helper->candidate = NULL;
        ++vc_helper; 
    }

    return vc;
}

void vc_destroy(vote_count_t vc)
{
    vote_count_t vc_helper = vc;

    // frees all candidate strings created using strdup_or_else()
    for (int i = 0; i < MAX_CANDIDATES; ++i) {
        free(vc_helper->candidate);
        ++vc_helper;
    }   
    
    free(vc);
}

size_t* vc_update(vote_count_t vc, const char* name)
{
    vote_count_t name_loc = vc_find_name(vc, name);
    // try to find the location of the given name
    
    if (!name_loc) { // !NULL = !0 = !false = true

        vote_count_t empty_loc = vc_find_empty(vc);
        // can't find name, so find the first NULL candidate
        
        if (!empty_loc) {   // can't find NULL candidate
            return NULL;
        }

        char* name_copy = strdup_or_else(name); 
        // malloc in strdup_or_else
        // name_copy has ownership 

        empty_loc->candidate = name_copy; 
        // ownership passed to candidate
        // will be freed in vc_destroy()

        empty_loc->count = 0;
        // intialize the count to 0

        return &(empty_loc->count);
    }

    return &(name_loc->count);
}

size_t vc_lookup(vote_count_t vc, const char* name)
{
    vote_count_t name_loc = vc_find_name(vc, name);
    
    if (!name_loc) { // return 0 if name can't be found
        return 0;
    }

    return name_loc->count;
}

size_t vc_total(vote_count_t vc)
{
    vote_count_t vc_helper = vc;
    size_t total = 0;
    for (int i = 0; i < MAX_CANDIDATES && 
        vc_helper->candidate != NULL; ++i) {
            total += vc_helper->count; 
            ++vc_helper;
    }
    return total;
}

const char* vc_max(vote_count_t vc)
{
    vote_count_t vc_helper = vc; 
    vote_count_t vc_max_pos = vc; // keeps track of the position of the current max count
    size_t max = 0; // keeps track of the current max count
    
    for (int i = 0; i < MAX_CANDIDATES &&
        vc_helper->candidate != NULL; ++i) {
            if (vc_helper->count > max) { // greater than (>) results in earliest candidate stored in case of a tie
                max = vc_helper->count;
                vc_max_pos = vc_helper; // updates to the position of current max count
            }
            ++vc_helper; 
        }

    if (max == 0) { // every candidate has 0 votes
        return NULL;
    }
    else {
        return vc_max_pos->candidate;
    }
}

const char* vc_min(vote_count_t vc)
{
    vote_count_t vc_helper = vc;
    vote_count_t vc_min_pos = vc;
    size_t min = 0;
    bool first_nonzero = true; // for a code snippet that runs once in a loop

    for (int i = 0; i < MAX_CANDIDATES &&
        vc_helper->candidate != NULL; ++i) {
            if (vc_helper->count == 0) {
                ++vc_helper;
                continue;
            }
            else if (first_nonzero) {       // if we encounter our first nonzero count
                min = vc_helper->count;     // set min to that count
                vc_min_pos = vc_helper;     // set vc_min_pos to that position
                first_nonzero = false;      // setting first_nonzero = false ensures this if branch does not run again 
                ++vc_helper;
                continue;
            }
            else if (vc_helper->count <= min) { // less or equal to (<=) results in
                min = vc_helper->count;         // latest candidate stored in case of a tie
                vc_min_pos = vc_helper;
            }
            ++vc_helper;
        }

    if (min == 0) { // every candidate has 0 votes
        return NULL;
    }
    else {
        return vc_min_pos->candidate;
    }
}

void vc_print(vote_count_t vc)
{
    vote_count_t vc_helper = vc;
    for (int i = 0; i < MAX_CANDIDATES &&
        vc_helper->candidate != NULL; ++i) {
        
        fprintf(stdout,"%-20s %9d\n", vc_helper->candidate, vc_helper->count);
        ++vc_helper;
    }
    
}

