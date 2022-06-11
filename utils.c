#include <time.h>

#include "utils.h"

//*********** FONCTION CATCH KEY PRESSED **************//
int keypressed()
{
  system( "/bin/stty raw");
  int key;
  system( "/bin/stty -echo");
  key = getc( stdin);
  system( "/bin/stty echo");
  system( "/bin/stty cooked");
  return key;
}



//*********** FONCTION TO CLEAN STDIN  **************//
void cleanStdin()
{
  int stdinChar;
  while(( stdinChar = getchar()) != EOF && stdinChar != '\n');
}



//
int startWith( char* str, char* substr
){
  int index = -1;
  for( int i = 0; str[i] != '\0'; i++
  ){
    index = -1;
    for( int j = 0; substr[j] != '\0'; j++
    ){
      if( str[i + j] != substr[j]
      ){
        index = -1;
        break;
      }
      index = i;
    }
    if( index != -1
    ){
      break;
    }
  }
  return( index != -1);
}

void displayTime( long timestamp, int shortDisplay, char* expirationMessage
){
  if( !shortDisplay
  ){
    struct tm* expirationTM = localtime( &timestamp);
    char expirationDate[20];
    strftime( expirationDate, 20, "%d/%m/%Y %H:%M:%S", expirationTM);

    printf( "%s", expirationDate);
  }

  int delaySeconds = difftime( timestamp, time( NULL));
  if( delaySeconds <= 0
  ){
    printf( "\n%s\n", expirationMessage);
    return;
  }

  int delayFullMinutes = (int)delaySeconds / 60;
  int delayMinutes     = delayFullMinutes;
  int delayHours       = (int)delayMinutes / 60;
  int delayDays        = (int)delayHours   / 24;

  if( delayDays == 0
  ){
    if( !shortDisplay
    ){
      printf( " - ");
    }
    delaySeconds -= delayMinutes * 60;
    delayMinutes -= delayHours   * 60;

    if(   delayHours       >  0
    ){
      if( delayMinutes     >  0
       && delayMinutes     < 10){ printf( "0");                 }
                                  printf( "%dh", delayHours  );
    }
    if(   delayMinutes     >  0){ printf( "%dm", delayMinutes); }
    if(   delayFullMinutes <  5
     &&   delaySeconds     >= 0 // display 00s everytime
    ){
      if( delayFullMinutes >  0
       && delaySeconds     < 10){ printf( "0"); }
                                  printf( "%ds", delaySeconds);
    }
  }
}


// ===============================================================
int verifyPassword( char* pwd)
{
	size_t length = strlen( pwd);

	if( length < PWD_CRITERIA_LENGTH
	){
	    return 0; // false;
	}

	size_t  upper_count = 0;
	size_t  lower_count = 0;
	size_t  digit_count = 0;
	size_t symbol_count = 0;

	for( int i = 0; i < length; i++
	){
	    if(      isupper( pwd[ i])) {  upper_count++; }
	    else if( islower( pwd[ i])) {  lower_count++; }
	    else if( isdigit( pwd[ i])) {  digit_count++; }
	    else if( ispunct( pwd[ i])) { symbol_count++; }
	    if(  upper_count >= PWD_CRITERIA_UPPER
	     &&  lower_count >= PWD_CRITERIA_LOWER
	     &&  digit_count >= PWD_CRITERIA_DIGIT
	     && symbol_count >= PWD_CRITERIA_SYMBOL
	    ){
	        return 1; // true;
	    }
	}
	return 0; // false;
}


// ===============================================================
void vigenereCrypt( char* phrase, char* key
){
	char* cypherKey =( key != NULL ? key : "CYTECHLIBRARY");

	for( int iChar = 0; iChar < strlen( phrase); iChar++
	){
		phrase[ iChar] = (( phrase[ iChar] - 32                                             // alphabet from ASCII 32 "space"
	                                       + cypherKey[ (int) iChar % strlen( cypherKey)] // switch to alphabet starting at key letter (0 to 12)
	                    ) - 32                   // minimal 32 "space" char
	                      +( 122 + 1 - 32)       // avoid negative value => push to next range (reduced by modulo below)
	                   ) % ( 122 + 1 - 32) + 32; // loop between 32 to 122
	}
}

void vigenereDecrypt( char* cypheredPhrase, char* key
){
	char* cypherKey =( key != NULL ? key : "CYTECHLIBRARY");

	for( int iChar = 0; iChar < strlen( cypheredPhrase); iChar++
	){
		cypheredPhrase[ iChar] =(( cypheredPhrase[ iChar] - cypherKey[ (int) iChar % strlen( cypherKey)] // alphabet from key letter (0 to 12)
	                                                    + 32                                           // back to alphabet from ASCII 32 "space"
	                           ) - 32                   // minimal 32 "space" char
	                             +( 122 + 1 - 32)       // avoid negative value => push to next range (reduced by modulo below)
	                          ) % ( 122 + 1 - 32) + 32; // loop between 32 to 122
	}
}
