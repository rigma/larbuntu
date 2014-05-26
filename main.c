#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <utils.h>

int main(void)
{
	static char menu = 0, run = 1;
	time_t t;
	struct tm *info;

	do
	{
		static char run2 = 1;
		do
		{
			system("cls");
			fflush(stdin);
			printf("Bienvenue sur le menu principal de la bibliotheque. Veuillez entrer le numero de l'action que vous voulez effectuer :\n\n0) Menu de Services \n1) Menu d'interface\n2) Quitter le programme\n\nVeuillez saisir votre choix : ");
		} while (!scanf("%d", &menu) || (menu > 2) || (menu < 0));

		if (!menu)
		{
			do
			{
				do
				{
					system("cls");
					fflush(stdin);
					printf("Vous etes dans le menu de services. Veuillez entrer le nombre correspondant a\nl'action souhaitee.\n\n0) Affichage des adherents\n1) Affichage des livres\n");
					printf("2) Affichage des adherent ayant depasser la date d'emprunt\n3) Recherche dans les bases de donnees\n4) Retour au menu principal\n\nVeuillez saisir votre choix : ");
				} while (!scanf("%d", &menu) || (menu > 4) || (menu < 0));

				switch (menu)
				{
				case 0:
					printf("Affichage des adherents en cours\n");
					break;

				case 1:
					printf("Affichage des livres en cours\n");
					break;

				case 2:
					printf("Affichage date emprunt en cours\n");
					break;
				case 3:
					printf("Recherche base de bonnee en cours\n");
					break;
				case 4:
					run2 = 0;
					break;
				}
			} while (run2);
			
		}
		else if (menu == 1)
		{
			do
			{
				do
				{
					system("cls");
					fflush(stdin);
					printf("Vous etes dans le menu d'interface. Veuillez entrer le nombre correspondant a\nl'action souhaitee.\n\n0) Affichage de la date\n1) Ajout d'un nouvel adherent ou d'un nouvea livre\n");
					printf("2) Supression d'un adherent ou d'un livre\n3) Gestion des emprunts de livres\n4) Gestion des restitutions de livre\n5) Retour au menu principal\n\nVeuillez saisir votre choix : ");
				} while (!scanf("%d", &menu) || (menu > 5) || (menu < 0));

				switch (menu)
				{
				case 0:
					system("cls");
					time(&t);
					info = localtime(&t);

					printf("%s\n\n\n\n\n", datetime(info));
					system("pause");
					break;

				case 1:

					break;

				case 2:
					printf("Supression d'un adherent ou d'un livre en cours\n");
					break;

				case 3:
					printf("Gestion des emprunts en cours\n");
					break;

				case 4:
					printf("Gestion des restitutions en cours\n");
					break;
				case 5:
					run2 = 0;
					break;
				}
			} while (run2);

		}

		else
		{
			run = 0;
		}
	} while (run);

	return EXIT_SUCCESS;
}
