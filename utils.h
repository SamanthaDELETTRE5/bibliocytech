#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//*********** FONCTIONS **************//
/**
  * keypressed
  *            - magic function to wait until a key is pressed before continuing business
  * output : integer - return ASCII value of key pressed
  */
int  keypressed();
/**
  * cleanStdin
  *            - another magic function to clean stdin buffer
  *            - especially usefull with usage of scanf
  */
void cleanStdin();
/**
  * startWith
  * input : a string and its substring - check if a string start with provided substring
  * output : response to that question (1/0) true/false
  */
int  startWith( char*, char*);
/**
  * displayTime
  * input : long timestamp value since 01/01:1970
  * input : integer flag to enable/disable display of full date or just delay before timeout
  * input : text - message to display in case of expired date
  */
void displayTime( long, int, char*);
/**
  * verifyPassword
  * input  : text - password to verify strong
  * output : response to that question (1/0) true/false
  * note : configuration CONST are provided here ( length, nb of upper, lower, digit and symbols )
  */
int  verifyPassword(         char*);
static const int PWD_CRITERIA_LENGTH = 8;
static const int PWD_CRITERIA_UPPER  = 1;
static const int PWD_CRITERIA_LOWER  = 1;
static const int PWD_CRITERIA_DIGIT  = 1;
static const int PWD_CRITERIA_SYMBOL = 1;

/**
  * vigenereCrypt
  * input : text - phrase to be cyphered
  * input : text - key used to cypher input phrase
  */
void vigenereCrypt(   char*, char*);
/**
  * vigenereDecrypt
  * CAUTION - never used it to decypher a password
  *         - can be used to decypher usernames, roles, etc...
  * input : text - phrase to be decyphered
  * input : text - key used to decypher input phrase
  */
void vigenereDecrypt( char*, char*);

#endif
