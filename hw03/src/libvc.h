/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

// YOU MUST NOT MODIFY THIS FILE.
//
// If you do, we may not be able to compile our tests against
// your code.

#pragma once

/*
 * This header describes a library for counting votes. It provides a
 * type, `vote_count_t`, that stores a mapping from candidate names to
 * vote counts, as well as operations for creating, updating, querying,
 * and destroying that type.
 */

#include <stddef.h>

// MAX_CANDIDATES is maximum number of different candidates we can count
// votes for. Adding candidates to the count will fail if we try to add
// more.
//
// The next three lines is a preprocessor conditional that only defines
// MAX_CANDIDATES if it is not already defined.
#ifndef MAX_CANDIDATES
#  define MAX_CANDIDATES  16
#endif

// This assertion is checked at compile time and will prevent the code
// from compiling if MAX_CANDIDATES isn't positive.
_Static_assert( MAX_CANDIDATES > 0, "MAX_CANDIDATES cannot be 0 or less" );

// Pointer to incomplete type. Clients of our code know that
// `vote_count_t` is a pointer (and won't accidentally mix it up with
// other types of pointers), but they don't know anything about the
// struct that it points to.
typedef struct vote_count* vote_count_t;

// Creates and returns a new, empty `vote_count_t` on the heap. Every
// successful call to `vc_create` allocates a new object that must
// subsequently be deallocated exactly once using `vc_destroy`.
//
// OWNERSHIP: The caller takes ownership of the result.
//
// ERRORS: Returns NULL if memory cannot be allocated.
vote_count_t vc_create(void);

// Deallocates all memory associated with `vc`.  `vc` may be NULL, in
// which case this function is a no-op.
//
// OWNERSHIP: The function takes ownership of `vc`.
//
// ERRORS: If `vc` has already been destroyed or wasn’t returned by
// `vc_create` in the first place then this function has undefined
// behavior.
void vc_destroy(vote_count_t vc);

// Returns a pointer to the count for candidate `name`. If `name` is
// already present in `vc` this will be the existing count; otherwise,
// `vc` is extended to map `name` to a count of 0 before the pointer to
// that count is returned.
//
// OWNERSHIP:
//  - Borrows `name` transiently, which means that it does not store it
//    anywhere. (In other words, `vc` must still be valid even after
//    `name` is not.)
//  - The function borrows `vc` transiently.
//  - The returned pointer is borrowed from `vc` and is valid until
//    `vc` is destroyed.
//
// ERRORS:
// - Returns NULL if `name` is not present in `vc` and cannot be
//   added because `vc` is full.
// - Prints a message to stderr and exits with code 1 if we need
//   to allocate a copy of `name` and allocation fails.
size_t* vc_update(vote_count_t vc, const char* name);

// Looks up the count for candidate `name`; returns 0 if not found.
//
// OWNERSHIP:
//  - Borrows both arguments transiently.
size_t vc_lookup(vote_count_t vc, const char* name);

// Returns the total number of votes cast.
//
// OWNERSHIP:
//  - Borrows `vc` transiently.
size_t vc_total(vote_count_t vc);

// Returns the name of the candidate with the most votes. In case of a
// tie, returns the candidate who was added to the `vc` *earlier*.
//
// Returns NULL if `vc` contains no candidates with more than zero
// votes.
//
// OWNERSHIP:
//  - Borrows `vc` transiently.
//  - The returned pointer is borrowed from `vc` and is valid until `vc`
//    is destroyed.
const char* vc_max(vote_count_t vc);

// Returns the name of the candidate with the fewest (non-zero) votes.
// In case of a tie, returns the candidate who was added to the `vc`
// *later*.
//
// Returns NULL if `vc` contains no candidates with more than zero
// votes.
//
// OWNERSHIP:
//  - Borrows `vc` transiently.
//  - The returned pointer is borrowed from `vc` and is valid until `vc`
//    is destroyed.
const char* vc_min(vote_count_t vc);

// Prints a summary of the vote counts on stdout. The counts are printed
// one candidate per line in the order they first were added. The
// candidate names are left-aligned in a 20-character column, followed
// by a single space, and then the counts right-aligned in a 9-character
// column.
//
// OWNERSHIP:
//  - Borrows `vc` transiently.
void vc_print(vote_count_t vc);
