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

#include "obooks.h"

//  Structure of our class

struct _book_t {
  char* id;
  char* title;
  char* author;
};

//  --------------------------------------------------------------------------
//  Create a new book
book_t *
book_new (const char* id, const char *author, const char *title)
{
  book_t *self = (book_t *) zmalloc (sizeof (book_t));
  assert (self);
  self->id = (char *)zmalloc(40);
  self->author = (char *)zmalloc(100);
  self->title = (char *)zmalloc(200);
  strcpy(self->id,id);
  strcpy(self->author, author);
  strcpy(self->title, title);
  return self;
} 

book_t *
book_new_session (const char *id, zpgutil_session_t *session)
{
    //  TODO: Initialize properties
    char* query = (char*)zmalloc(100);
    sprintf(query, "SELECT author, title FROM book WHERE id='%s';",id);
    zpgutil_session_sql (session, query);
    PGresult* res = zpgutil_session_select (session);
    book_t *self = book_new_pg (res);
    PQclear(res);
    return self;
}

book_t *
book_new_pg (PGresult *res)
{
    book_t *self = (book_t *) zmalloc (sizeof (book_t));
    assert (self);
    if(PQntuples(res)==0) 
    {
    zsys_debug ("No tuple returned !");
    return NULL;
    }
    else 
    {
    self->id = (char *)zmalloc(40);
    self->title = (char *)zmalloc(200);
    self->author = (char*)zmalloc(100);
    strcpy(self->author,PQgetvalue(res,0,0));
    strcpy(self->title,PQgetvalue(res,0,1));
    }
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
        free (self->id);
        free (self->title);
        free (self->author);
 
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
    printf ("%s (%s)\n", self->title, self->author);
}


char*
book_get_title (book_t *self)
{
  char *title = (char *)zmalloc(200);
  if(self->title) 
  {
  strcpy(title,self->title);
  }
  return title;
}

char*
book_get_author (book_t *self)
{
  char *author = (char *)zmalloc(200);
  if(self->author) 
  {
  strcpy(author,self->author);
  }
  return author;
}

//  --------------------------------------------------------------------------
//  Selftest

int
book_test (bool verbose)
{
    printf (" * book: ");
    zconfig_t* config = zconfig_load(".testdir/test.cfg");
    assert(config); 
    zpgutil_datasource_t *datasource = zpgutil_datasource_new (config);
    assert (datasource);
    zpgutil_session_t *session = zpgutil_session_new (datasource);
    assert (session);
 
    //  @selftest
    //  Simple create/destroy test
    book_t *self = book_new_session ("1", session);
    assert (self);
    char *author = book_get_author(self);
    printf("--%s",author);
    assert (author);
    assert (strcmp(author,"Victor Hugo")==0);
    book_print (self);
    book_destroy (&self);
    //  @end

    printf ("OK\n");
    return 0;
}
