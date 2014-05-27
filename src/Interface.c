#include <Interface.h>
#include <stdlib.h>
#include <stdio.h>

void interface_Capture(db_t *db)
{
	member_t* member = NULL;
	thema_t* thema = NULL;
	book_t* book = NULL;
	static char test = 0;

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
	else // ecriture livre
	{
		book = book_register(NULL);
		//entrée dans la base de donnée
	}
}
