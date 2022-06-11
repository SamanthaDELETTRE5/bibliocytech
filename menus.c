#include "menus.h"


//*********** TITRE ACCUEIL **************//

void accueil( User* user)
{
  printf( "\nBonjour");
  if( user != NULL) {
    printf(" %s", user->username);
  }
  printf(", bienvenue dans notre bibliothèque !\n\n");
}

//*********** MENU CONNEXION **************//

void menuConnexion()
{
  accueil( NULL);

  printf( "\n1. Se connecter\n");
  printf(   "2. S'inscrire\n");
  printf(   "3. Quitter l'application\n");
  printf( "\n> ");
}

//*********** MENU ACTIONS **************//

void menuActions( User* user)
{
  accueil( user);

  if( user->nbTotalBooks > 0
  ){
    printf( "\nVous empruntez actuellement %d livre%s.\n", user->nbTotalBooks, user->nbTotalBooks > 1 ? "s" : "");
  }
  if( strlen( user->expiredBook) > 0
  ){
    printf( "\nAu moins un de vos livres est à rendre au plus tôt !!!\nVous ne pouvez actuellement plus emprunter de nouveau livre.\n");
  }

  if( user->role == 1)
  {
    printf( "\nQue voulez-vous faire ?\n");
    if( user->nbTotalBooks >= 5
    ){
      printf( " 1. Nombre maximal de livres empruntés atteint (%d)\n", user->nbTotalBooks);
    } else
    if( strlen( user->expiredBook) > 0
    ){
      printf( " 1. D'anciens emprunts non rendus bloquent tout nouvel emprunt\n");
    } else
    {
      printf( " 1. Emprunter un livre\n");
    }
    printf( " 2. Rendre un livre\n");
    printf( " 3. Ajouter un livre\n");
    printf( " 4. Se déconnecter\n");
  }
  if( user->role == 2)
  {
    printf( "\nQue souhaitez-vous ?\n");
    if( user->nbTotalBooks >= 3
    ){
      printf( " 1. Nombre maximal de livres empruntés atteint (%d)\n", user->nbTotalBooks);
    } else
    if( strlen( user->expiredBook) > 0
    ){
      printf( " 1. D'anciens emprunts non rendus bloquent tout nouvel emprunt\n");
    } else
    {
      printf( " 1. Emprunter un livre\n");
    }
    printf( " 2. Rendre un livre\n");
    printf( " 3. Se déconnecter\n");
  }
  printf( "\n> ");
}

//*********** MENU EMPRUNTER **************//

void menuEmprunter()
{
  printf( "\n 101. Trier par categories\n");
  printf( " 102. Reduire la liste des livres en selectionnant une categorie\n");
  printf( " 103. Trier par auteurs\n");
  printf( " 104. Reduire la liste des livres en selectionnant un auteur\n");
  printf( " 105. Trier par titres\n");
  printf( " 106. Sortie\n");
  printf( "\nou : entrez le numero du livre choisi\n> ");
}
