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
  zpgutil_session_t *session;
};

//  --------------------------------------------------------------------------
//  Create a new shelf

shelf_t *
shelf_new(zpgutil_session_t *session)
{
 shelf_t *self = (shelf_t *) zmalloc (sizeof (shelf_t));
 assert (self);

  //  TODO: Initialize properties
 self->session = session;


 return self;
}

int
count_books (shelf_t *self)
{
return 0;
}

void
get_books (shelf_t *self, zlist_t *target)
{
}

book_t*
add_book (shelf_t *self, const char *author, const char *title)
{
  char *sql = (char*)zmalloc(500);
  zuuid_t *uuid = zuuid_new ();
  const char *str_uuid = zuuid_str (uuid);
  sprintf(sql,"INSERT INTO book(id,author,title) VALUES('%s','%s','%s');",str_uuid,author,title);
  zpgutil_session_sql (self->session, sql);
  zpgutil_session_execute (self->session);
  zpgutil_session_commit (self->session);     
  book_t *book = book_new (str_uuid,self->session);
  return book;
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
    zconfig_t* config = zconfig_load(".testdir/test.cfg");
    assert(config); 
    zpgutil_datasource_t *datasource = zpgutil_datasource_new (config);
    assert (datasource);
    zpgutil_session_t *session = zpgutil_session_new (datasource);
    assert (session);
 
    //  @selftest
    //  Simple create/destroy test
    shelf_t *self = shelf_new (session);
    assert (self);
    book_t *mybook = shelf_add (self,"Albert Camus","L'étranger");
    assert (mybook);
    shelf_destroy (&self);
    //  @end

    printf ("OK\n");
    return 0;
}
