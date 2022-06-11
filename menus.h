#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"

//*********** FONCTIONS **************//
/**
  * accueil - a generic welcome message to be reused/same everywhere
  * input : User - if provided, a nominative welcome will be displayed
  */
void accueil( User*);
/**
  * menuConnexion
  *               - user will have possibility to login, register or quit
  */
void menuConnexion();
/**
  * menuActions
  *               - once connected, user will have access to several functions
  * input: User - according to role, user will have some VIP actions
  *             - or depending of numbers of borrowed books, or expiration date
  *             - may some acions have been blocked
  */
void menuActions( User*);
/**
  * menuEmprunter
  *               - for books management, display list of possible actions
  *               - like order, filter, quit or select a specific book reference number
  */
void menuEmprunter();

#endif