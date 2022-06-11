#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "books.h"
#include "menus.h"
#include "users.h"
#include "utils.h"

//*********** MAIN **************//
int main()
{
  int choice, choice2;
  User* user = malloc( sizeof( User));

  int orderBy = 0;
  // default: // 2: title
  // 3 - author
  // 4 - category

  // load users - done later
  int nbrUsers;
  User* usersList = NULL;

  // load books - done after login
  int nbrBooks;
  Book* booksList = NULL;
  int result;

  do
  {
    // load users - on each action to update changes
    nbrUsers = getNbrUser();
    if( usersList != NULL) { free( usersList); }
    usersList = malloc( nbrUsers * sizeof( User));
    getUsersList( &usersList);

    system("clear");
    menuConnexion();
    choice = keypressed() - '0';

    if( choice == 1 // login
    ){
      login( user, usersList, nbrUsers);
      do
      {
        // load books - on each action to update changes
        nbrBooks = getNbrBook();
        if( booksList != NULL){ free( booksList); }
        booksList = malloc( nbrBooks * sizeof( Book));
        result = loadBookList( user, &booksList);

        system("clear");
        menuActions( user);
        choice2 = keypressed() - '0';

        if( choice2 == 1 // borrow a new book
        ){
          // prevent user actions if not granted
          if((( user->role == 1 && user->nbTotalBooks < 5)
            ||( user->role == 2 && user->nbTotalBooks < 3)
             )
           && strlen( user->expiredBook) == 0
          ){
            borrowBook( user, booksList, orderBy, nbrBooks);
          }
        }
        if( choice2 == 2 // return a borrowed book
        ){
          returnBook( user, booksList, orderBy, nbrBooks);
        }
        if( choice2 == 3    // add a new book
         && user->role == 1 // only for professors
        ){
          addBook();
        }
      } while(( user->role == 1 && choice2 != 4)   // professor exit on 4
            ||( user->role == 2 && choice2 != 3)); // student on 3
    }
    if( choice == 2 // register a new account
    ){
      addUser();
    }
  } while( choice != 3); // close application

  system( "clear");
  return 0;
}
