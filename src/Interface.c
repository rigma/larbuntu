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
