#include <Interface.h>
#include <stdlib.h>
#include <stdio.h>

void interface_Capture()
{
	member_t* member = NULL;
	char test = 0;

	do
	{
		system("cls");
		fflush(stdin);
		printf("Voulez vous ajoutez un adherent ou un livre ?\n0) Adherent\n1) Livre");
	} while (!scanf("%d", &test) || (test > 1) || (test < 0));

	if (!test) //ecriture adherent
	{
		member = member_register(NULL);
	}
	else // ecriture livre
	{

	}
}
