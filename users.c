#include "users.h"
#include "utils.h"


//*********** FONCTION ADD USER **************//
void addUser() {
  User a;

  printf( "\nTapez votre nom d'utilisateur\n");
  fgets( a.username, 256, stdin);
  a.username[ strcspn( a.username, "\r\n")] = 0; // supprime les retours à la ligne
//  scanf( "%s", a.username);
  do
  {
    printf( "\nTapez votre mot de passe\n");
    fgets( a.password, 256, stdin);
    a.password[ strcspn( a.password, "\r\n")] = 0; // supprime les retours à la ligne
//    scanf( "%s", a.password);
    if( verifyPassword( a.password) == 0
    ){
      printf( "Votre mot de passe n'est pas suffisamment protecteur !\nD'une longueur minimum de 8 caracteres, ajoutez au moins 1 majuscule, minuscule, chiffre ET un symbole !!!\n");
      printf( "continue...\n");
      keypressed();
      continue;
    }
    break;
  } while( 1);
  vigenereCrypt( a.password, NULL);
  vigenereCrypt( a.username, a.password);

  printf( "\n1: professeur 2: élève\n");
  scanf( "%d", &a.role);
  cleanStdin();

  FILE* usersFile = NULL;
  usersFile = fopen( "users.txt", "a"); // ouverture du fichier
  if ( usersFile == NULL) {
    printf( "addUser - problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur = %s\n", strerror( errno));
    exit( 1);
  }
  fprintf( usersFile, "%s|%s|%d\n", a.username, a.password, a.role);
  fclose( usersFile);
  printf( "Inscription réussie\n");
  printf( "continue...");
  keypressed();
}



//*********** COUNT a LIST OF USERS **************//
int getNbrUser() { 
  int nmbUser = 0;
  char nmblines[2000];
  FILE* usersFile = NULL;
  usersFile = fopen( "users.txt", "r");
  if ( usersFile == NULL) {
    printf( "getNbrUser - problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur = %s\n", strerror( errno));
    exit( 1);
  }

  while ( fgets(nmblines, 1999, usersFile) != NULL) {
    nmbUser++;
  }
  fclose( usersFile);
  return nmbUser;
}



//*********** Return a LIST OF USERS **************//
int getUsersList( User* *tab) {

  FILE* usersFile = NULL;
  usersFile = fopen( "users.txt", "r");
  if ( usersFile == NULL) {
    printf( "getUsersList - problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur = %s\n", strerror( errno));
    exit( 1);
  }

  int tabIndex = 0;

  char*  line = calloc( 256, sizeof( char));
  int    wordPosition = 0;
  char*  word;
  char* pWord = NULL; // keep word initial position for free it later

  while( fgets( line, 256, usersFile))
  {
    line[ strcspn( line, "\r\n")] = 0; // supprime les retours à la ligne
    // need to reinstiate word - shrunk by strtok successive calls
    wordPosition = 0;
    word = calloc( 256, sizeof( char));
    pWord = word;
    strcpy( word, line);

    strtok( word, "|");
    while( word != NULL)
    {
      wordPosition++;
      if( wordPosition == 1) { strcpy( (*tab)[ tabIndex].username,    word); }
      if( wordPosition == 2) { strcpy( (*tab)[ tabIndex].password,    word); }
      if( wordPosition == 3) {         (*tab)[ tabIndex].role = atoi( word); }
      word = strtok( NULL, "|");
    }
    tabIndex++;
    if( pWord != NULL) { free( pWord); pWord = NULL; }
  }
  if( line != NULL) { free( line); line = NULL; }
  fclose( usersFile);
  return 1;
}



//*********** User verification **************//
int login( User* userAccount, User* tab, int nmbUser)
{
  char givenUsername[256] = "";
  char givenPassword[256] = "";
  int  loginSuccess = 0;
  do {
    printf(   "Connexion\n");
    printf( "\nQuel est votre nom d'utilisateur ?\n");
    fgets( givenUsername, 256, stdin);
    givenUsername[ strcspn( givenUsername, "\r\n")] = 0; // supprime les retours à la ligne
//    scanf( "%s", givenUsername);
    printf(   "Quel est votre mot de passe ?\n");
    fgets( givenPassword, 256, stdin);
    givenPassword[ strcspn( givenPassword, "\r\n")] = 0; // supprime les retours à la ligne
//    scanf( "%s", givenPassword);
//    cleanStdin();

    vigenereCrypt( givenPassword, NULL);
    char cypheredUsername[256];
    strcpy( cypheredUsername, givenUsername);
    vigenereCrypt( cypheredUsername, givenPassword);

    for( int i = 0; i < nmbUser; i++) {
      if( strcmp( cypheredUsername, tab[i].username) == 0
       && strcmp( givenPassword,    tab[i].password) == 0
      ){
        loginSuccess = 1;
        strcpy( (*userAccount).username, givenUsername);
        (*userAccount).role = tab[i].role;
        printf( "\nFélicitations %s, vous êtes connecté(e) !\n", givenUsername);
        printf( "continue...");
        keypressed();
        break;
      }
    }
  } while( loginSuccess == 0);

  return 1;
}



