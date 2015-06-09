/*  =========================================================================
    shelf - 
        Represents a Shelf (containing a set of Books) 
        
        

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
    shelf - 
        Represents a Shelf (containing a set of Books) 
        
        
@discuss
@end
*/

#include "../include/obooks.h"

//  Structure of our class

struct _shelf_t {
  char *name;
};

//  --------------------------------------------------------------------------
//  Create a new shelf

shelf_t *
shelf_new(const char *name, zpgutil_session_t *session)
{
    shelf_t *self = (shelf_t *) zmalloc (sizeof (shelf_t));
    assert (self);

    //  TODO: Initialize properties

    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the shelf

void
shelf_destroy (shelf_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        shelf_t *self = *self_p;

        //  Free class properties

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Print properties of object

void
shelf_print (shelf_t *self)
{
    assert (self);
}


//  --------------------------------------------------------------------------
//  Selftest

int
shelf_test (bool verbose)
{
    printf (" * shelf: ");

    //  @selftest
    //  Simple create/destroy test
    shelf_t *self = shelf_new ("my shelf",NULL);
    assert (self);
    shelf_destroy (&self);
    //  @end

    printf ("OK\n");
    return 0;
}
