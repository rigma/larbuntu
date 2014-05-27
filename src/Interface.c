#include <Interface.h>
#include <stdlib.h>
#include <stdio.h>

void interface_Capture(db_t *db)
{
	member_t* member = NULL;
	book_t* book = NULL;
	static char test = 0;

	do
	{
		system("cls");
		fflush(stdin);
		printf("Voulez vous ajoutez un adherent ou un livre ?\n\n0) Adherent\n1) Livre\n\n");
		printf("Veuillez saisir votre reponse : ");
	} while (!scanf("%d", &test) || (test > 1) || (test < 0));

	if (!test) //ecriture adherent
	{
		member = member_register(NULL);
		member_add(db->db_member, member);
	}
	else // ecriture livre
	{
		book = book_register(NULL);
		//entrée dans la base de donnée
	}
}
