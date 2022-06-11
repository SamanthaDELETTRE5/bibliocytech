#include <time.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "books.h"
#include "menus.h"
#include "utils.h"

//*********** Count number of books available **************//
int getNbrBook()
{
  int nmbBook = 0;
  char nmblines[3000]; 
  FILE* booksFile = NULL;
  booksFile = fopen( "books.txt", "r");
  if ( booksFile == NULL) {
    printf( "problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur = %s\n", strerror( errno));
    exit(1);
  }
  while ( fgets( nmblines, 1999, booksFile) != NULL) {
    nmbBook++;
  }
  return nmbBook;
}



//*********** Return a list of Books **************//
int loadBookList( User* user, Book* *ioTab)
{
  FILE* booksFile = fopen( "books.txt", "r");
  if( booksFile == NULL)
  {
    printf( "problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur : %s\n", strerror( errno));
    exit(1);
  }

  int tabIndex = 0;
  user->nbTotalBooks = 0;
  strcpy( (*user).expiredBook, "");

  char*  line = calloc( 256, sizeof( char));
  int    wordPosition = 0;
  char*  word;
  char* pWord = NULL; // keep word initial position for free it later

  while( fgets( line, 256, booksFile))
  {
    line[ strcspn( line, "\r\n")] = 0; // supprime les retours à la ligne
    // need to reinstiate word - shrunk by strtok successive calls
    wordPosition = 0;
    word = calloc( 256, sizeof( char));
    pWord = word;
    strcpy( word, line);

    strcpy( (*ioTab)[ tabIndex].username,  "");
    strcpy( (*ioTab)[ tabIndex].timestamp, "");

    strtok( word, "|");
    while( word != NULL)
    {
      wordPosition++;
      if( wordPosition == 1) { strcpy( (*ioTab)[ tabIndex].id,        word); }
      if( wordPosition == 2) { strcpy( (*ioTab)[ tabIndex].title,     word); }
      if( wordPosition == 3) { strcpy( (*ioTab)[ tabIndex].author,    word); }
      if( wordPosition == 4) { strcpy( (*ioTab)[ tabIndex].category,  word); }
      if( wordPosition == 5) { strcpy( (*ioTab)[ tabIndex].username,  word); }
      if( wordPosition == 6) { strcpy( (*ioTab)[ tabIndex].timestamp, word); }
      word = strtok( NULL, "|");
    }
    if( strlen( user->username) > 0
     && strlen( (*ioTab)[ tabIndex].username) > 0
     && strcmp( user->username, (*ioTab)[ tabIndex].username) == 0
    ){
      user->nbTotalBooks++;
      if( difftime( atoi( (*ioTab)[ tabIndex].timestamp), time( NULL)) <= 0
      ){
        strcpy( (*user).expiredBook, (*ioTab)[ tabIndex].id);
      }
    }
    tabIndex++;
    if( pWord != NULL) { free( pWord); pWord = NULL; }
  }
  if( line != NULL) { free( line); line = NULL; }

  fclose( booksFile);

  return 1;
}



//*********** Switch **************//
void switchBooksOrder( Book* pa, Book* pb
){
  Book tmp;
  tmp = *pa;
  *pa = *pb;
  *pb = tmp;
}

//*********** SHOW BOOKS **************//
void displayBooksList( User* user, Book* a, int orderBy, int filterBy, char* filterValue, int nmbBooks)
{
  sortBooksList( &a, orderBy, nmbBooks);

  int maxLenghTitle    = 50; // todo: 1ere passe pour calculer les longueurs
  int maxLenghAuthor   = 20; // todo: fixer le pb des caractères accentués
  int maxLenghCategory = 14;
  char lastFilteredField[256] = "";

  if( filterBy == orderBy
   && filterBy == 4
  ){
    printf( "\nListe des categories:\n");
  } else
  {
    printf( "\nListe des livres:\n");
  }

  for( int i = 0; i < nmbBooks; i++)
  {
    if( filterBy == orderBy) // filter + ordered => display list of categories
    {
      if( filterBy == 3
       && strcmp( lastFilteredField, a[i].author) != 0 // distinct ordered values
      ){
        maxLenghAuthor = fmax( maxLenghAuthor, strlen( a[i].author));

        strcpy( lastFilteredField, a[i].author);
        printf( " %*s", maxLenghAuthor, a[i].author);
        printf( "\n");

        continue; // next for iteration
      }
      if( filterBy == 4
       && strcmp( lastFilteredField, a[i].category) != 0 // distinct ordered values
      ){
        maxLenghCategory = fmax( maxLenghCategory, strlen( a[i].category));

        strcpy( lastFilteredField, a[i].category);
        printf( " %*s", maxLenghCategory, a[i].category);
        printf( "\n");

        continue; // next for iteration
      }
    }
    if(( user == NULL && strlen( a[i].username) == 0 // no user => all books not borrowed
      || user != NULL && strcmp( a[i].username, user->username) == 0 // only books borrowed by this user
       )
     &&(  filterBy == 0 // default => display all title
      ||( filterBy == 3 // filter  => display title of that author
       && filterValue != NULL
       && strlen( filterValue) > 0
       && strcmp( filterValue, a[i].author) == 0
        )
      ||( filterBy == 4 // filter  => display title of that category
       && filterValue != NULL
       && strlen( filterValue) > 0
       && strcmp( filterValue, a[i].category) == 0
    ))){
      maxLenghTitle    = fmax( maxLenghTitle,    strlen( a[i].title));
      maxLenghAuthor   = fmax( maxLenghAuthor,   strlen( a[i].author));
      maxLenghCategory = fmax( maxLenghCategory, strlen( a[i].category));

                           printf(  " %s",                    a[i].id);
                           printf( "\t%*s", maxLenghTitle,    a[i].title);
      if( filterBy != 3) { printf( "\t%*s", maxLenghAuthor,   a[i].author);   }
      if( filterBy != 4) { printf( "\t%*s", maxLenghCategory, a[i].category); }

      if( strlen( a[i].timestamp) > 0
      ){
        printf( "\t");
        displayTime( atoi( a[i].timestamp), 1, " !!! Borrow Time Expired !!!");
      }

      printf( "\n");
    }
  }
}

//*********** ALPHABETICAL SORT **************//
void sortBooksList( Book* *tab2, int orderBy, int nmbBooks) {
  int triOn = 0;
  int i = 0;
  int index = 0;
  int atLeastOneSwitch = 0;

  char first, second;
  
  while( i < nmbBooks - 1
  ){
    if( orderBy == 3) // 3: author
    {
      first  = (*tab2)[i  ].author[index];
      second = (*tab2)[i+1].author[index];
    } else
    if( orderBy == 4) // 4: category
    {
      first  = (*tab2)[i  ].category[index];
      second = (*tab2)[i+1].category[index];
    } else // default // 2: title
    {
      first  = (*tab2)[i  ].title[index];
      second = (*tab2)[i+1].title[index];
    }
    if( first == '\0'
    ){
      i++; // trié => passe ligne suivante
      index = 0;
    } else
    if( second == '\0'
     || tolower( first) > tolower( second)
    ){
      atLeastOneSwitch = 1;
      switchBooksOrder( &(*tab2)[i], &(*tab2)[i+1]);
      i++; // trié => passe ligne suivante
      index = 0;
    } else
    if( tolower( first) == tolower( second)
    ){
      index++; // sinon on teste le caractère suivant
    } else
    {
      i++;
      index = 0;
    }
    if( i == nmbBooks - 1)
    {
      i = ++triOn;
    }
    if( i == nmbBooks - 1
     && atLeastOneSwitch == 1
    ){
      atLeastOneSwitch = 0;
      triOn = 0;
      i = triOn;
    }
    // comment on fait quand il y a des _ entre les différents mots d'un titre ????
  }
}

//*********** SEARCH BY AUTHOR **************//
void searchBooksByAuthor( Book tab2[], int nmbBooks, char* author)
{
  displayBooksList( NULL, tab2, 0, 3, author, nmbBooks);
}



//*********** SEARCH BY CATEGORY **************//
void searchBooksByCategory( Book tab2[], int nmbBooks, char* cat)
{
  displayBooksList( NULL, tab2, 0, 4, cat, nmbBooks);
}

//*********** BORROW **************//
int borrowBook( User* user, Book* booksTab, int orderBy, int nmbBooks)
{
  int choice, choice2;
  char filterValue[256] = "";
  int intOrderBy = orderBy;
  int intFilterBy = 0;

  do
  {
    system("clear");
    displayBooksList( NULL, booksTab, intOrderBy, intFilterBy, filterValue, nmbBooks);

    if( intOrderBy >= 3
     && intOrderBy == intFilterBy
    ){
      intOrderBy = 0;
      if( intFilterBy == 4){ printf( "\nEntrez la categorie: ");       } else
      if( intFilterBy == 3){ printf( "\nEntrez le nom de l'auteur: "); }
      fgets( filterValue, 256, stdin);
//      scanf("%s", filterValue);
      cleanStdin();
      continue; // while
    }

    intFilterBy = 0;
    strcpy( filterValue, "");

    menuEmprunter();
    scanf("%d", &choice);
    cleanStdin();
    if( choice == 101){ intOrderBy  = 4; } else
    if( choice == 102){ intOrderBy  = 4;
                        intFilterBy = 4; } else
    if( choice == 103){ intOrderBy  = 3; } else
    if( choice == 104){ intOrderBy  = 3;
                        intFilterBy = 3; } else
    if( choice == 105){ intOrderBy  = 0;
                        intFilterBy = 0; }
    if( choice == 106){ return 1; }
  } while( choice > 100);

  FILE* booksFile     = fopen( "books.txt", "r");
  FILE* booksTempFile = fopen( "booksTemp.txt", "w");
  if( booksFile == NULL
   || booksTempFile == NULL
  ){
    printf( "borrowBook - problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur : %s\n", strerror( errno));
    if( booksFile     != NULL){ fclose( booksFile);     }
    if( booksTempFile != NULL){ fclose( booksTempFile); }
    exit(1);
  }

  time_t timestamp = time( NULL);
  struct tm *expirationTM = localtime( &timestamp);
  (*expirationTM).tm_min +=( user->role == 1 ? 3 : 2);
  timestamp = mktime( expirationTM);
  char expirationDate[20];
  strftime( expirationDate, 20, "%d/%m/%Y %H:%M:%S", expirationTM);

  int foundBook = 0;
  char*  line = calloc( 256, sizeof( char));
  int    wordPosition = 0;
  char*  word;
  char* pWord = NULL; // keep word initial position for free it later

  while( fgets( line, 256, booksFile)
  ){
    line[ strcspn( line, "\r\n")] = 0; // supprime les retours à la ligne
    // need to reinstiate word - shrunk by strtok successive calls
    wordPosition = 0;
    word = calloc( 256, sizeof( char));
    pWord = word;
    strcpy( word, line);

    strtok( word, "|");
    char bookId[256];       strcpy( bookId,       word); word = strtok( NULL, "|");
    char bookTitle[256];    strcpy( bookTitle,    word); word = strtok( NULL, "|");
    char bookAuthor[256];   strcpy( bookAuthor,   word); word = strtok( NULL, "|");
    char bookCategory[256]; strcpy( bookCategory, word); word = strtok( NULL, "|");
    char username[256] = "";
    if( word != NULL) {
      strcpy( username, word); word = strtok( NULL, "|");
    }
    if( foundBook == 0
     && atoi( bookId) == choice
     && strlen( username) == 0
    ){
      foundBook = 1;
      fprintf( booksTempFile, "%s|%s|%"PRId64"\n", line, user->username, timestamp);
    } else
    {
      fprintf( booksTempFile, "%s\n", line);
    }
    if( pWord != NULL) { free( pWord); pWord = NULL; }
  }
  if( line != NULL) { free( line); line = NULL; }

  fclose( booksTempFile);
  fclose( booksFile);

  if( foundBook == 0
  ){
    printf( "\nLe livre '%d' n'est pas connu de notre collection !?\n", choice);
    remove( "booksTemp.txt");
  } else
  if( remove( "books.txt") != 0
  ){
    perror( "\nThe books file can not be deleted");
  } else
  if( rename( "booksTemp.txt", "books.txt") != 0
  ){
    perror( "\nThe modified books file can not be renamed");
  } else
  {
    printf( "\nLa reference de ce livre '%d' vous est maintenant attribuee !\n", choice);
    printf( "\nPrenez-en soin et pensez a nous le restituer avant la date du %s\n", expirationDate);
    printf(   "Au dela de cette date, certains acces de la bibliotheque pourront vous etre retires !\n");
    printf( "\nMerci par avance et Bonne lecture.\n");
  }
  printf( "\ncontinue...");
  keypressed();

  return 1;  
}

//*********** RETURN **************//
int returnBook( User* user, Book* booksTab, int orderBy, int nmbBooks)
{
  int choice, choice2;
  char filterValue[256] = "";
  int intOrderBy = orderBy;
  int intFilterBy = 0;

  do
  {
    system("clear");
    displayBooksList( user, booksTab, intOrderBy, intFilterBy, filterValue, nmbBooks);

    if( intOrderBy >= 3
     && intOrderBy == intFilterBy
    ){
      intOrderBy = 0;
      if( intFilterBy == 4){ printf( "\nEntrez la categorie: ");       } else
      if( intFilterBy == 3){ printf( "\nEntrez le nom de l'auteur: "); }
      fgets( filterValue, 256, stdin);
//      scanf("%s", filterValue);
      cleanStdin();
      continue; // while
    }

    intFilterBy = 0;
    strcpy( filterValue, "");

    menuEmprunter();
    scanf("%d", &choice);
    cleanStdin();
    if( choice == 101){ intOrderBy  = 4; } else
    if( choice == 102){ intOrderBy  = 4;
                        intFilterBy = 4; } else
    if( choice == 103){ intOrderBy  = 3; } else
    if( choice == 104){ intOrderBy  = 3;
                        intFilterBy = 3; } else
    if( choice == 105){ intOrderBy  = 0;
                        intFilterBy = 0; }
    if( choice == 106){ return 1; }
  } while( choice > 100);

  FILE* booksFile     = fopen( "books.txt", "r");
  FILE* booksTempFile = fopen( "booksTemp.txt", "w");
  if( booksFile == NULL
   || booksTempFile == NULL
  ){
    printf( "borrowBook - problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur : %s\n", strerror( errno));
    if( booksFile     != NULL){ fclose( booksFile);     }
    if( booksTempFile != NULL){ fclose( booksTempFile); }
    exit(1);
  }

  int foundBook = 0;
  char*  line = calloc( 256, sizeof( char));
  int    wordPosition = 0;
  char*  word;
  char* pWord = NULL; // keep word initial position for free it later

  while( fgets( line, 256, booksFile)
  ){
    line[ strcspn( line, "\r\n")] = 0; // supprime les retours à la ligne
    // need to reinstiate word - shrunk by strtok successive calls
    wordPosition = 0;
    word = calloc( 256, sizeof( char));
    pWord = word;
    strcpy( word, line);

    strtok( word, "|");
    char bookId[256];       strcpy( bookId,       word); word = strtok( NULL, "|");
    char bookTitle[256];    strcpy( bookTitle,    word); word = strtok( NULL, "|");
    char bookAuthor[256];   strcpy( bookAuthor,   word); word = strtok( NULL, "|");
    char bookCategory[256]; strcpy( bookCategory, word); word = strtok( NULL, "|");
    char username[256] = "";
    if( word != NULL) {
      strcpy( username, word); word = strtok( NULL, "|");
    }
    if( foundBook == 0
     && atoi( bookId) == choice
     && strcmp( username, user->username) == 0
    ){
      foundBook = 1;
      fprintf( booksTempFile, "%s|%s|%s|%s\n", bookId, bookTitle, bookAuthor, bookCategory);
    } else
    {
      fprintf( booksTempFile, "%s\n", line);
    }
    if( pWord != NULL) { free( pWord); pWord = NULL; }
  }
  if( line != NULL) { free( line); line = NULL; }

  fclose( booksTempFile);
  fclose( booksFile);

  if( foundBook == 0
  ){
    printf( "\nLe livre '%d' n'est pas connu de notre collection !?\n", choice);
    remove( "booksTemp.txt");
  } else
  if( remove( "books.txt") != 0
  ){
    perror( "\nThe books file can not be deleted");
  } else
  if( rename( "booksTemp.txt", "books.txt") != 0
  ){
    perror( "\nThe modified books file can not be renamed");
  } else
  {
    printf( "\nLa reference de ce livre '%d' nous a bien été rendue !\n", choice);
    printf( "\nMerci et bonne journée.\n");
  }
  printf( "\ncontinue...");
  keypressed();

  return 1;  
}

//*********** ADD A BOOK **************//
void addBook()
{
  FILE* booksFile = fopen( "books.txt", "r");
  if( booksFile == NULL)
  {
    printf( "problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur : %s\n", strerror( errno));
    exit(1);
  }

  int newReference = 0;

  char*  line = calloc( 256, sizeof( char));
  char*  word;
  char* pWord = NULL; // keep word initial position for free it later

  while( fgets( line, 256, booksFile))
  {
    line[ strcspn( line, "\r\n")] = 0; // supprime les retours à la ligne
    // need to reinstiate word - shrunk by strtok successive calls
    word = calloc( 256, sizeof( char));
    pWord = word;
    strcpy( word, line);

    newReference = fmax( newReference, atoi( word));

    if( pWord != NULL) { free( pWord); pWord = NULL; }
  }
  if( line != NULL) { free( line); line = NULL; }

  fclose( booksFile);

  Book a;
  if( newReference < 10
  ){
    sprintf( a.id, "00%d", newReference + 1);
  } else
  if( newReference < 100
  ){
    sprintf( a.id, "0%d", newReference + 1);
  } else
  {
    sprintf( a.id, "%d", newReference + 1);
  }

//  printf( "\nChoisissez un identifiant pour votre livre\n"); // le mettre en random ? 
//  scanf( "%s", a.id);
  printf( "\nQuel est le titre du livre\n");
  fgets( a.title, 256, stdin);
  a.title[ strcspn( a.title, "\r\n")] = 0; // supprime les retours à la ligne
//  scanf( "%s", a.title);
  printf( "\nQuel est l'auteur\n");
  fgets( a.author, 256, stdin);
  a.author[ strcspn( a.author, "\r\n")] = 0; // supprime les retours à la ligne
//  scanf( "%s", a.author);
  printf( "\nChoisissez la catégorie de votre livre:");
  printf( "\n 1. Littérature");
  printf( "\n 2. Politique");
  printf( "\n 3. Histoire");
  printf( "\n 4. Sciences");
  printf( "\n 5. Anglais");
  printf( "\n 6. Economie");
  printf( "\n> ");
  scanf( "%s", a.category);

  if( strcmp( a.category, "1") == 0){ strcpy( a.category, "Littérature"); } else
  if( strcmp( a.category, "2") == 0){ strcpy( a.category, "Politique"  ); } else
  if( strcmp( a.category, "3") == 0){ strcpy( a.category, "Histoire"   ); } else
  if( strcmp( a.category, "4") == 0){ strcpy( a.category, "Sciences"   ); } else
  if( strcmp( a.category, "5") == 0){ strcpy( a.category, "Anglais"    ); } else
  if( strcmp( a.category, "6") == 0){ strcpy( a.category, "Economie"   ); }

  FILE* fichier = NULL;
  fichier = fopen( "books.txt", "a");
  if( fichier == NULL) {
    printf( "problème d'ouverture\n");
    printf( "code d'erreur : %d\n", errno);
    printf( "message d'erreur : %s\n", strerror( errno));
    exit( 1);
  }

  fprintf( fichier, "%s|%s|%s|%s\n", a.id, a.title, a.author, a.category);
  printf( "\nLe livre a été ajouté avec succès");
  printf( "\ncontinue...");
  keypressed();

  fclose( fichier);
}