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
  PGresult *all_books_res;
  int row;
};

//  --------------------------------------------------------------------------
//  Create a new shelf

shelf_t *
shelf_new(zpgutil_session_t *session)
{
 shelf_t *self = (shelf_t *) zmalloc (sizeof (shelf_t));
 assert (self);
 //----
  //  TODO: Initialize properties
 self->session = session;
 self->row = 0;
 return self;
}

int
shelf_count_books (shelf_t *self)
{
  char *sql = "SELECT COUNT(*) FROM book";
  zpgutil_session_sql (self->session, sql);
  char* res = zpgutil_session_select_one (self->session);
  assert (res);
  int a = atoi(res);
  free (res);
  return a;
}

void
shelf_load_books (shelf_t *self)
{
  char *sql = "SELECT id, author, title FROM book";
  zpgutil_session_sql (self->session, sql);
  self->all_books_res = zpgutil_session_select (self->session);
  assert (self->all_books_res);
  self->row = 0;
  return; 
}

book_t *
shelf_next_book (shelf_t *self)
{
 int size = PQntuples(self->all_books_res);
 if((self->all_books_res!=NULL) && (PQresultStatus(self->all_books_res)==PGRES_TUPLES_OK) && (self->row<size))
 {
  zsys_info ("get %i out of %i \n", self->row+1, size);
  char *id = PQgetvalue (self->all_books_res, self->row, 0);
  assert (id);
  char *author = PQgetvalue (self->all_books_res, self->row, 1);
  assert (author);
  char *title = PQgetvalue (self->all_books_res, self->row, 2);
  assert (title);
  book_t *book = book_new (id, author, title);
  assert (book);
  zsys_info ("get object ok ! \n");
  self->row = self->row + 1;
  return book;
 }
 zsys_error ("could not get next object !\n");
 return NULL;
} 

book_t*
shelf_add_book (shelf_t *self, const char *author, const char *title)
{
  zuuid_t *uuid = zuuid_new ();
  const char *str_uuid = zuuid_str (uuid);
  char* sql = "INSERT INTO book(id,author,title) VALUES($1,$2,$3);"; 
  zpgutil_session_sql (self->session, sql);
  zpgutil_session_set (self->session, (char*)str_uuid);
  zpgutil_session_set (self->session, (char*)author);
  zpgutil_session_set (self->session, (char*)title);
  zpgutil_session_execute (self->session);
  zpgutil_session_commit (self->session);     
  book_t *book = book_new (str_uuid,author,title);
  zuuid_destroy (&uuid); 
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
        if(self->all_books_res) 
        {
        PQclear (self->all_books_res);
        }
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
    book_t *mybook = shelf_add_book (self,"William Shakespeare","Macbeth");
    assert (mybook);
    book_destroy (&mybook);

    int c = shelf_count_books (self);
    assert (c>0);

    shelf_load_books (self);
    book_t *loaded;
    while ( (loaded = shelf_next_book (self)) != NULL)
    {
      book_print (loaded);
      book_destroy (&loaded);
    }
    shelf_destroy (&self);
    //  @end

    zpgutil_session_destroy (&session);
    zpgutil_datasource_destroy (&datasource);
    zconfig_destroy (&config);

    printf ("OK\n");
    return 0;
}
