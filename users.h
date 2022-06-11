#ifndef USERS_H_INCLUDED
#define USERS_H_INCLUDED

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//*********** STRUCT **************//
typedef struct {
  int  role;
  char username[100];
  char password[100];
  int nbTotalBooks;
  char expiredBook[256];
}
User;

//*********** FUNCTIONS **************//
/**
  * addUser
  *         - for self-registration
  *         - have to provide username, password and to choose a role
  */
void addUser();
/**
  * getNbrUser
  * output : integer - return number of users registered
  */
int  getNbrUser();
/**
  * loadBookList
  * input/output : User[] - list of registered users
  * output : integer - return (1/0) for success or failure
  */
int  getUsersList( User**);
/**
  * login
  * input/output : User - user account with all needed informations to be used later
  * input        : User[] - list of registered users
  * input        : ineger - number of registered users
  * output       : integer - return (1/0) for success or failure
  */
int  login( User*, User*, int);

#endif // USERS_H_INCLUDED
