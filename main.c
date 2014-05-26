#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	int menu = 0;
	do
	{
		printf("Bienvenu sur le menu principal de la bibliotheque. Veuillez entrer le numero du menu que vous voulez utiliser :\n0) Menu de Services \n1) Menu interface\n");
		scanf("%d", &menu);
	} while ((menu > 2) || (menu < -1));
	if (!menu)
	{
		printf("42\n");
	}
	else
	{
		printf("LEEEEEEROOYYYYYYY JEEEEEEENKIIIIINS\n");
	}
	system("pause");

	return EXIT_SUCCESS;
}
