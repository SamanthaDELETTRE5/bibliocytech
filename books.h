#ifndef BOOKS_H_INCLUDED
#define BOOKS_H_INCLUDED

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"

//*********** STRUCT **************//
typedef struct {
  char        id[256];
  char     title[256];
  char    author[256];
  char  category[256];
  char  username[256]; // name of user who borrow that book - exclusive
  char timestamp[256]; // date in future when book will have to be returned
}
Book;

//*********** FONCTIONS **************//
/**
  * getNbrBook
  * output : integer - return number of books registered
  */
int  getNbrBook();
/**
  * loadBookList
  * input/output : User - will be completed with number of books borrowed
  *                                   and reference of first expired book
  * input/output : Book[] - list of registered books
  * output : integer - return (1/0) for success or failure
  */
int  loadBookList( User*, Book**);
/**
  * switchBooksOrder
  * input/output : two books - exchange their pointers position if books arrays
  */
void switchBooksOrder( Book*, Book*);
/**
  * displayBooksList
  * input : User - if provided, restrict list to user's borrowed books only
  * input : Book[] - list of registered books
  * input : integer orderBy - used to display books according to specified field
  *                           0 is default
  *                           3 for authors
  *                           4 for categories
  * input : integer filterBy - used to reduce list of displayed books to specified field
  *                            combined to orderBy => 3 will display all known authors
  *                                                => 4 will display all known categories
  *                            combined with filterValue
  * input : text filterValue - value associated to specified filterBy
  * input : integer nmbBooks - used to loop all registered books
  */
void displayBooksList( User*, Book*, int, int, char*, int);
/**
  * sortBooksList
  * input/output : Book[] - list of registered books to order
  * input : integer orderBy - used to display books according to specified field
  *                           0 is default
  *                           3 for authors
  *                           4 for categories
  * input : integer nmbBooks - used to loop all registered books
  */
void sortBooksList( Book**, int, int);
/**
  * searchBooksByCategory
  *                       - this is specialized function of displayBooksList for Categories
  *                       - orderBy default 0
  *                       - filterBy valuated Category
  * input : Book[] - list of registered books
  * input : integer nmbBooks - used to loop all registered books
  * input : text filterValue - value associated to specified filterBy
  */
void searchBooksByCategory( Book*, int, char*);
/**
  * searchBooksByCategory
  *                       - this is specialized function of displayBooksList for Authors
  *                       - orderBy default 0
  *                       - filterBy valuated Author
  * input : Book[] - list of registered books
  * input : integer nmbBooks - used to loop all registered books
  * input : text filterValue - value associated to specified filterBy
  */
void searchBooksByAuthor(   Book*, int, char*);
/**
  * borrowBook
  * input : User - user informations to complete book reservation
  * input : Book[] - list of registered books
  * input : integer orderBy - initial order to display books
  * input : integer nmbBooks - used to loop all registered books
  */
int  borrowBook( User*, Book*, int, int); 
/**
  * returnBook
  * input : User - user informations to check/valid good return of borrowed book
  * input : Book[] - list of registered books
  * input : integer orderBy - initial order to display books
  * input : integer nmbBooks - used to loop all registered books
  * output : integer - return (1/0) for success or failure
  */
int  returnBook( User*, Book*, int, int);
/**
  * addBook
  */
void addBook();

#endif // BOOKS_H_INCLUDED
