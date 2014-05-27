#include <Interface.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void interface_Capture(db_t *db)
{
	member_t* member = NULL;
	thema_t* thema = NULL;
	book_t* book = NULL;

	static int *available = NULL;
	static char key[8] = { 0 };
	static char test = 0;

	static int i = 0;

	do
	{
		system("cls");
		fflush(stdin);
		printf("Que voulez-vous ajouter?\n\n0) Un adherent\n1) Un theme\n2) Un livre\n\n");
		printf("Veuillez saisir votre reponse : ");
	} while (!scanf("%d", &test) || (test > 2) || (test < 0));

	if (test == 0) //ecriture adherent
	{
		member = member_register(NULL);
		member_add(db->db_member, member);
	}
	else if (test == 1)
	{
		thema = thema_register(NULL);
		thema_add(db->db_thema, thema);
	}
	else if (test == 2) // ecriture livre
	{
		book = book_register(NULL);

		if (db->db_thema->size == 0)
		{
			book_free(book);
			printf("Il n'y a pas de themes enregistres dans la base de donnees.\n");

			return;
		}
		
		available = (int*) malloc((db->db_thema->size - db->db_thema->deleted) * sizeof(int));
		thema = db->db_thema->first;

		printf("Ajoutez le livre dans quel theme ?\n\n");
		for (i = 0 ; thema != NULL ; i++)
		{
			printf("%d) %s\n", i, thema->key);
			available[i] = thema->id;

			thema = thema->next;
		}

		printf("\n");

		do
		{
			fflush(stdin);
			printf("Effectuez votre choix : ");
		} while (!scanf("%d", &test) || test < 0 || test > i);

		thema_insertBook(db->db_thema->themas[available[test]], book);
		book_add(db->db_book, book);

		free(available);
	}
}

void interface_Delete(db_t *db)
{
	member_t* member = NULL;
	thema_t* thema = NULL;
	book_t* book = NULL;
	static char test = 0;
	static char answer = 0;
	static unsigned int id = 0;

	

	do
	{
		fflush(stdin);
		printf("Que voulez-vous supprimmer?\n\n0) Un adherent\n1) Un theme\n2) Un livre\n\n");
		printf("Veuillez saisir votre reponse : ");
	} while (!scanf("%d", &test) || (test > 2) || (test < 0));

	if (test == 0) //ecriture adherent
	{

		printf("Veuillez entrer l'ID de la personne a suprimmer (cette valeur est disponible dans l'affichage des adherents) : ");
		scanf("%.10d", id);
		member = db->db_member->first;
		while (member != NULL)
		{
			if (id == member->id)
			{
				do
				{
					printf("Voulez vous supprimmez les donnees concernant %s %s ?\n\n0 = Oui\n1 = Non\n\n Veuillez saisir votre reponse : ", member->name, member->forname);
				} while (!scanf("%d", &answer) || (answer > 1) || (answer < 0));
				if (answer == 0)
				{

					member_remove(db->db_member, member->id);
					// partie ou on sort de la boucle.
				}
				
			}
			member = member->next;
		}
	}
	else if (test == 1)
	{

	}
	else if (test == 2) // ecriture livre
	{

	}
}