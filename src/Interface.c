#include <Interface.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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
	static char test = 0;
	static unsigned int id = 0;
	static char key[4] = { 0 }, tmp[4] = { 0 };
	static char code[8] = { 0 };
	static unsigned short i = 0;

	do
	{
		fflush(stdin);
		printf("Que voulez-vous supprimmer?\n\n0) Un adherent\n1) Un theme\n2) Un livre\n\n");
		printf("Veuillez saisir votre reponse : ");
	} while (!scanf("%d", &test) || (test > 2) || (test < 0));

	if (test == 0) //ecriture adherent
	{
		system("cls");
		printf("Veuillez entrer l'ID de la personne a suprimmer (cette valeur est disponible dans l'affichage des adherents) : ");
		scanf("%d", &id);

		member = db->db_member->first;
		while (member != NULL)
		{

			if (id == member->id)
			{
				do
				{
					printf("Voulez vous supprimmez les donnees concernant %s %s ?\n\n0 = Oui\n1 = Non\n\n Veuillez saisir votre reponse : ", member->name, member->forname);
				} while (!scanf("%d", &test) || (test > 1) || (test < 0));

				if (test == 0)
					member_remove(db->db_member, member->id);

				return;
				
			}
			member = member->next;
		}
	}
	else if (test == 1) // supression theme
	{
		system("cls");
		printf("Veuillez entrer la cle du theme a suprimmer (cette valeur est disponible dans l'affichage des themes) : ");
		scanf("%s", key); 
		fflush(stdin);

		thema = db->db_thema->first;
		while (thema != NULL)
		{

			if (!strcmp(key, thema->key))
			{
				do
				{
					printf("Voulez vous supprimmez les donnees concernant %s ?\n\n0 = Oui\n1 = Non\n\n Veuillez saisir votre reponse : ", thema->key);
				} while (!scanf("%d", &test) || (test > 1) || (test < 0));

				if (test == 0)
					thema_remove(db->db_thema, thema->id);

				return;

			}
			thema = thema->next;
		}
	}
	else if (test == 2) // suppression du livre 
	{
		system("cls");
		printf("Veuillez entrer le code du livre a supprimer (cette valeur est disponible dans l'affichage des livres) : ");
		scanf("%s", code);
		fflush(stdin);

		strncpy(key, code, 3);
		tmp[0] = code[4]; 
		tmp[1] = code[5];
		tmp[2] = code[6];

		thema = db->db_thema->first;
		while (thema != NULL)
		{

			if (!strcmp(key, thema->key))
			{
				for (i = 0; i < thema->size; i++)
				{
					if (thema->books[i]->entry == atoi(tmp))
					{
						do
						{
							printf("Voulez vous supprimmez les donnees concernant %s ?\n\n0 = Oui\n1 = Non\n\n Veuillez saisir votre reponse : ", thema->books[i]->title);
						} while (!scanf("%d", &test) || (test > 1) || (test < 0));

						if (test == 0)
						{
							book_remove(db->db_book, thema->books[i]->id);
							break;
						}

					}
				}

				return;

			}
			thema = thema->next;
		}
	}
}

void interface_Borrow(db_t *db)
{
	book_t *book = NULL;
	member_t* member = NULL;
	thema_t* thema = NULL;

	static char test = 0;
	static unsigned int id = 0;
	static unsigned char duration = 0;
	static unsigned short i = 0;
	static unsigned int j = 0;

	static char key[4] = { 0 }, tmp[4] = { 0 }, entry[3] = { 0 };
	static char code[10] = { 0 };

	printf("Veuillez entrer l'ID de la personne souhaitant emprunter un livre (cette valeur est disponible dans l'affichage des adherents) : ");
	scanf("%d", &id);

	printf("Veuillez entrer la cle du theme du livre emprunter (cette valeur est disponible dans l'affichage des themes) : ");
	scanf("%s", key);
	fflush(stdin);

	member = db->db_member->first;
	thema = db->db_thema->first;
	strncpy(key, code, 3);
	tmp[0] = code[4];
	tmp[1] = code[5];
	tmp[2] = code[6];

	while (member != NULL)
	{

		if (id == member->id)
		{
			do
			{
				printf("Est-ce bien %s %s qui souhaite emprunter un livre ??\n\n0 = Oui\n1 = Non\n\n Veuillez saisir votre reponse : ", member->name, member->forname);
			} while (!scanf("%d", &test) || (test > 1) || (test < 0));

			if (test == 0)
			{
				if (member->n_borrows == MAX_BORROWS_MEMBER)
					printf("Ce membre a deja atteint le nombre maximal d'emprunt.\n");
				else
				{
					while (thema != NULL)
					{
						if (!strcmp(key, thema->key))
						{
							for (i = 0 ; i < thema->size ; i++)
							{
								if (thema->books[i]->entry == atoi(tmp))
								{
									do
									{
										printf("Est-ce %s que l'adherent souhaite emprunter ?\n\n0 = Oui\n1 = Non\n\n Veuillez saisir votre reponse : ", thema->books[i]->title);
									} while (!scanf("%d", &test) || (test > 1) || (test < 0));

									if (test == 0 && thema->books[i]->free > 0)
									{
										book = thema->books[i];

										for (j = 0; j < book->effective; j++)
										{
											if (book->d_borrows[j] == 0)
											{
												do
												{
													fflush(stdin);
													printf("Duree de l'emprunt (15 jours maximum) : ");
												} while (!scanf("%d", &duration) || duration > 15);

												book->d_borrows[j] = time(NULL) + duration * 24 * 3600;

												break;
											}
										}
										
										sprintf(entry, "-%d", j);
										strcat(code, entry);
										strcpy(member->borrows[member->n_borrows], code);

										book->free--;
										member->n_borrows++;

										break;
									}

								}
							}

							return;
						}

						thema = thema->next;
					}
				}
			}

			return;

		}
		member = member->next;
	}
}