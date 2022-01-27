#include "../src/libvc.h"

#include <211.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_create_destroy(void)
{
    vc_destroy(vc_create());
}

static void test_creation(void)
{
    vote_count_t vc = vc_create();
    CHECK_SIZE( vc_lookup(vc, "joe"), 0 );


    size_t* cp = vc_update(vc, "joe");
    CHECK( cp );
    *cp += 1;
    CHECK_SIZE( vc_lookup(vc, "joe"), 1 );
    
    vc_destroy(vc);
}

static void test_2_candidates(void)
{
    // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 2) {
        return;
    }

    size_t* cp;
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    CHECK_POINTER( vc_max(vc), NULL );
    CHECK_POINTER( vc_min(vc), NULL );
    CHECK_SIZE( vc_lookup(vc, "alice"), 0 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 0 );

    cp = vc_update(vc, "alice");
    CHECK( cp );
    *cp += 1;
    CHECK_SIZE( vc_lookup(vc, "alice"), 1);
    CHECK_STRING( vc_max(vc), "alice" );
    CHECK_STRING( vc_min(vc), "alice" );
    CHECK_SIZE( vc_lookup(vc, "alice"), 1 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 1 );

    cp = vc_update(vc, "bob");
    CHECK( cp );
    *cp += 1;
    cp = vc_update(vc, "alice");
    CHECK( cp );
    *cp += 1;
    CHECK_STRING( vc_max(vc), "alice" );
    CHECK_STRING( vc_min(vc), "bob" );
    CHECK_SIZE( vc_lookup(vc, "alice"), 2 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 1 );
    CHECK_SIZE( vc_total(vc), 3 );

    vc_print(vc);
    vc_destroy(vc);
}

static void test_min_max(void) {
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    size_t* cp = vc_update(vc, "alice");
    CHECK( cp );

    cp = vc_update(vc, "bob");
    *cp = 21;

    cp = vc_update(vc, "joe");
    *cp = 10;

    cp = vc_update(vc, "dylan");
    *cp = 21;

    cp = vc_update(vc, "hank");
    *cp = 10;

    cp = vc_update(vc, "nut");

    CHECK_SIZE( vc_lookup(vc, "alice"), 0);
    CHECK_SIZE( vc_lookup(vc, "hank"), 10 );
    CHECK_STRING( vc_max(vc), "bob" );
    CHECK_STRING( vc_min(vc), "hank" );
    CHECK_SIZE( vc_total(vc), 62);
    vc_destroy(vc);
}

static void test_lookup(void) {
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    size_t* cp;
    
    CHECK_SIZE( vc_lookup(vc, "dill"), 0);
    vc_update(vc, "janus");
    CHECK_SIZE( vc_lookup(vc, "dill"), 0);
    cp = vc_update(vc, "dill ");
    CHECK( cp );
    (*cp)++;
    
    CHECK_SIZE( vc_lookup(vc, "dill"), 0);
    CHECK_SIZE( vc_lookup(vc, "dill "), 1);
    
    
    vc_destroy(vc);
}

int main(void)
{
    test_create_destroy();
    test_2_candidates();
    test_creation();
    test_min_max();
    test_lookup();
    return 0;
}

