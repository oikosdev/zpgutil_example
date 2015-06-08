/*  =========================================================================
    book - 
        Represents a Book 
        
        

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of CZMQ, the high-level C binding for 0MQ:       
    http://czmq.zeromq.org.                                            
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

/*
@header
    book - 
        Represents a Book 
        
        
@discuss
@end
*/

#include "obooks_library.h"

//  Structure of our class

struct _book_t {
  char* title;
  char* author;
};

//  --------------------------------------------------------------------------
//  Create a new book
book_t *
book_new (const char *title, const char *author, zpgutil_session_t *session)
{
    book_t *self = (book_t *) zmalloc (sizeof (book_t));
    assert (self);

    //  TODO: Initialize properties

    return self;
}

//  --------------------------------------------------------------------------
//  Destroy the book

void
book_destroy (book_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        book_t *self = *self_p;

        //  Free class properties

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Print properties of object

void
book_print (book_t *self)
{
    assert (self);
}


//  --------------------------------------------------------------------------
//  Selftest

int
book_test (bool verbose)
{
    printf (" * book: ");

    //  @selftest
    //  Simple create/destroy test
    book_t *self = NULL; //book_new ("Toilers of the sea", "Victor Hugo", NULL);
    assert (self);
    book_destroy (&self);
    //  @end

    printf ("OK\n");
    return 0;
}
