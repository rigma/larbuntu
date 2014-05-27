#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dbio.h>
#include <member.h>

member_t *member_init()
{
	member_t *member = (member_t*) malloc(sizeof(member_t));
	if (member == NULL)
		return NULL;

	member->name = NULL;
	member->forname = NULL;
	member->adress = NULL;
	member->email = NULL;
	member->profession = NULL;
	member->n_borrows = 0;
	member->borrows = NULL;
	member->previous = NULL;
	member->next = NULL;
	
	return member;
}

void member_free(member_t *member)
{
	if (member != NULL)
	{
		free(member->name);
		free(member->forname);
		free(member->profession);
		free(member->borrows);
		free(member);
	}
}

char member_add(member_db *db, member_t *member)
{
	member_t *previous = NULL, *next = NULL;
	member_t **tmp = NULL;
	unsigned int i = 0;

	if (db == NULL || member == NULL)
		return 0;

	previous = (db->next == 0) ? NULL : db->members[db->next - 1];
	next = (db->next >= db->size) ? NULL : db->members[db->next + 1];

	if (previous != NULL)
	{
		previous->next = member;
		member->previous = previous;
	}

	if (next != NULL)
	{
		next->previous = member;
		member->next = next;
	}

	member->id = db->next;

	if (db->size == 0)
		db->first = member;

	if (db->deleted == 0)
	{
		db->size++;
		tmp = (member_t**)realloc(db->members, db->size * sizeof(member_t*));

		if (tmp != NULL)
			db->members = tmp;
		else
			return 0;

		db->members[db->next] = member;
		db->next = db->size;
	}
	else
	{
		db->members[db->next] = member;
		db->deleted--;

		for (i = 0; i < db->size; i++)
		{
			if (db->members[i] == NULL)
			{
				db->next = i;

				break;
			}
		}

		if (i == db->size)
			db->next = db->size;
	}

	return 1;
}

char member_remove(member_db *db, unsigned int index)
{
	member_t *member = NULL, *previous = NULL, *next = NULL;

	if (db == NULL || index >= db->size)
		return 0;

	member = db->members[index];

	if (member == NULL)
		return 0;
	else
	{
		previous = member->previous;
		next = member->next;

		previous->next = next;
		next->previous = previous;

		db->members[index] = NULL;
		member_free(member);

		db->next = index;
		db->deleted++;

		if (db->size - db->deleted == 0)
			db->first = NULL;
	}

	return 1;
}

member_t* member_register(member_t* member)
{
	system("cls");
	member_t *m = NULL;
	char tempo[200];

	if (member == NULL)
		m = member_init();
	else
		m = member;

	printf(" --- Saisi d'un nouveau membre ---\n");

	fflush(stdin);
	printf("Veuillez entrer le nom de l'adherent : ");
	fgets(tempo, sizeof(tempo), stdin);
	
	tempo[strlen(tempo) - 1] = '\0';
	m->name = (char*) malloc((1 + strlen(tempo)) * sizeof(char));
	strcpy(m->name, tempo);

	fflush(stdin);
	printf("Veuillez entrer le prenom de l'adherent : ");
	fgets(tempo, sizeof(tempo), stdin);

	tempo[strlen(tempo) - 1] = '\0';
	m->forname = (char*) malloc((1 + strlen(tempo)) * sizeof(char));
	strcpy(m->forname, tempo);

	m->adress = adress_register(NULL);
	m->email = email_register(NULL);

	fflush(stdin);
	printf("Veuillez entrer la profession de l'adherent : ");
	fgets(tempo, sizeof(tempo), stdin);
	
	tempo[strlen(tempo) - 1] = '\0';
	m->profession = (char*) malloc((1 + strlen(tempo)) * sizeof(char));
	strcpy(m->profession, tempo);

	m->n_borrows = 0;
	m->borrows = NULL;

	return m;
}

member_db *member_initDatabase(char *name)
{
	// Variables de travail
	FILE *f = NULL;
	member_db *db = (member_db*) malloc(sizeof(member_db));
	member_t *member = NULL, *previous = NULL;
	char *filename = (char*) malloc((strlen(name) + 8) * sizeof(char));
	unsigned int i = 0;
	unsigned char j = 0;

	// Tampons de lecture
	char *buffer_str = NULL;
	unsigned short buffer_short = 0;
	unsigned char buffer_char = 0;

	if (db == NULL || filename == NULL)
	{
		if (db != NULL)
			free(db);
		else if (filename != NULL)
			free(filename);

		return NULL;
	}

	strcpy(filename, "db/");
	strcat(filename, name);
	strcat(filename, ".ldb");

	f = fopen(filename, "rb");
	if (f == NULL)
	{
		db->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
		if (db->name == NULL)
		{
			free(db);
			free(filename);

			return NULL;
		}
		
		strcpy(db->name, name);

		db->size = 0;
		db->next = 0;
		db->deleted = 0;
		db->first = NULL;
		db->members = NULL;
	}
	else
	{
		// On vérifie que la base de données est correcte
		buffer_str = (char*) malloc(9 * sizeof(char));
		if (buffer_str == NULL)
		{
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}

		fread(buffer_str, sizeof(char), 9, f);
		fread(&buffer_char, sizeof(unsigned char), 1, f);

		if (strcmp(buffer_str, LARBUNTU) || buffer_char != MEMBER)
		{
			printf("Base de données non valide.\n");

			free(buffer_str);
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}

		free(buffer_str);
		buffer_str = NULL;

		db->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
		if (db->name == NULL)
		{
			free(db);
			free(filename);
			fclose(f);

			return NULL;
		}

		strcpy(db->name, name);

		fread(&db->size, sizeof(unsigned int), 1, f);
		fread(&db->next, sizeof(unsigned int), 1, f);
		fread(&db->deleted, sizeof(unsigned int), 1, f);

		// Création du tableau contenant les adresses des membres de la bdd
		db->members = (member_t**) malloc(db->size * sizeof(member_t*));
		if (db->members == NULL)
		{
			free(db->name);
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}
		else
		{
			for (i = 0 ; i < db->size ; i++)
				db->members[i] = NULL;
		}

		for (i = 0 ; i < db->size ; i++)
		{
			// Lecture si l'entrée est valide
			fread(&buffer_char, sizeof(unsigned char), 1, f);
			if (buffer_char == VALID)
			{
				member = member_init();

				// Lecture de l'identifiant
				fread(&member->id, sizeof(unsigned int), 1, f);

				// Lecture du nom de l'adhérent
				fread(&buffer_short, sizeof(unsigned short), 1, f);
				member->name = (char*) malloc(buffer_short * sizeof(char));
				if (member->name == NULL)
				{
					member_free(member);
					
					for (i = 0 ; i < db->size ; i++)
						member_free(db->members[i]);

					free(db->members);
					free(db->name);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				fread(member->name, sizeof(char), buffer_short, f);

				// Lecture du prénom de l'adhérent
				fread(&buffer_short, sizeof(unsigned short), 1, f);
				member->forname = (char*) malloc(buffer_short * sizeof(char));
				if (member->forname == NULL)
				{
					member_free(member);

					for (i = 0 ; i < db->size ; i++)
						member_free(db->members[i]);

					free(db->members);
					free(db->name);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				fread(member->forname, sizeof(char), buffer_short, f);

				// Lecture de l'adresse postale de l'adhérent
				member->adress = (adress_t*) malloc(sizeof(adress_t));
				if (member->adress == NULL)
				{
					member_free(member);

					for (i = 0; i < db->size; i++)
						member_free(db->members[i]);

					free(db->members);
					free(db->name);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				fread(&member->adress->number, sizeof(unsigned short), 1, f);
				
				fread(&buffer_char, sizeof(unsigned char), 1, f);

				if (buffer_char == 0)
					member->adress->path = TRAIL;
				else if (buffer_char == 1)
					member->adress->path = ROAD;
				else if (buffer_char == 2)
					member->adress->path = DEADEND;
				else if (buffer_char == 3)
					member->adress->path = STREET;
				else if (buffer_char == 4)
					member->adress->path = AVENUE;
				else if (buffer_char == 5)
					member->adress->path = BOULEVARD;
				else if (buffer_char == 6)
					member->adress->path = PLACE;

				fread(&buffer_short, sizeof(unsigned short), 1, f);

				member->adress->pathname = (char*) malloc(buffer_short * sizeof(char));
				fread(member->adress->pathname, sizeof(char), buffer_short, f);

				fread(&member->adress->postal, sizeof(unsigned int), 1, f);

				fread(&buffer_short, sizeof(unsigned short), 1, f);

				member->adress->city = (char*) malloc(buffer_short * sizeof(char));
				fread(member->adress->city, sizeof(char), buffer_short, f);

				// Lecture de l'adresse courriel de l'adhérent
				member->email = (email_t*) malloc(sizeof(email_t));
				if (member->email == NULL)
				{
					member_free(member);

					for (i = 0; i < db->size; i++)
						member_free(db->members[i]);

					free(db->members);
					free(db->name);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				fread(&buffer_short, sizeof(unsigned short), 1, f);

				member->email->id = (char*) malloc(buffer_short * sizeof(char));
				fread(member->email->id, sizeof(char), buffer_short, f);

				fread(&buffer_short, sizeof(unsigned short), 1, f);

				member->email->hostname = (char*) malloc(buffer_short * sizeof(char));
				fread(member->email->hostname, sizeof(char), buffer_short, f);

				// Lecture de la profession de l'adhérent
				fread(&buffer_short, sizeof(unsigned short), 1, f);
				member->profession = (char*) malloc(buffer_short * sizeof(char));
				if (member->profession == NULL)
				{
					member_free(member);

					for (i = 0; i < db->size; i++)
						member_free(db->members[i]);

					free(db->members);
					free(db->name);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				fread(member->profession, sizeof(char), buffer_short, f);

				// Lecture et vérification du nombre d'emprunt
				fread(&member->n_borrows, sizeof(unsigned char), 1, f);
				if (member->n_borrows > MAX_BORROWS_MEMBER)
				{
					printf("Données corrompues !\n");

					member_free(member);

					for (i = 0; i < db->size; i++)
						member_free(db->members[i]);

					free(db->members);
					free(db->name);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				// Lecture des codes d'emprunt
				member->borrows = (char**) malloc(MAX_BORROWS_MEMBER * sizeof(char*));
				if (member->borrows == NULL)
				{
					member_free(member);

					for (i = 0; i < db->size; i++)
						member_free(db->members[i]);

					free(db->members);
					free(db->name);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				if (member->n_borrows > 0)
				{
					for (j = 0; j < member->n_borrows; j++)
					{
						member->borrows[j] = (char*)malloc(BORROWS_CODE * sizeof(char));

						fread(member->borrows[i], sizeof(char), BORROWS_CODE, f);
					}
				}

				// Création des liens de la chaîne
				if (i == 0)
					db->first = member;
				else
				{
					previous->next = member;
					member->previous = previous;
				}

				db->members[member->id] = member;
				previous = member;
			}
		}
	}

	free(buffer_str);
	free(filename);

	if (f != NULL)
		fclose(f);

	return db;
}

char member_saveDatabase(member_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	member_t *member = NULL;
	char *filename = (char*) malloc((8 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;
	unsigned char j = 0;

	// Tampons d'écriture
	char *buffer_str = NULL;
	unsigned short buffer_short = 0;
	unsigned char buffer_char = 0;

	if (db == NULL || filename == NULL)
		return 0;

	// Ouverture du fichier
	strcpy(filename, "db/");
	strcat(filename, db->name);
	strcat(filename, ".ldb");

	f = fopen(filename, "wb");
	if (f == NULL)
	{
		free(filename);

		return 0;
	}

	// Ecriture de l'entête
	buffer_str = (char*)malloc(9 * sizeof(char));
	if (buffer_str == NULL)
	{
		free(filename);
		fclose(f);

		return 0;
	}

	strcpy(buffer_str, LARBUNTU);
	fwrite(buffer_str, sizeof(char), 9, f);

	free(buffer_str);
	buffer_str = NULL;

	buffer_char = MEMBER;
	fwrite(&buffer_char, sizeof(unsigned char), 1, f);

	// Ecriture des informations sur la taille de base de données
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);
	fwrite(&db->deleted, sizeof(unsigned int), 1, f);

	for (i = 0 ; i < db->size ; i++)
	{
		member = db->members[i];

		if (member == NULL)
		{
			// Lorsque l'entrée est vide
			buffer_char = BAD;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
		}
		else
		{
			// Lorsque l'entrée n'est pas vide
			buffer_char = VALID;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			// Ecriture de l'entrée dans la base de données
			fwrite(&member->id, sizeof(unsigned int), 1, f);

			// Ecriture du prénom de l'adhérent
			buffer_short = strlen(member->name) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->name, sizeof(char), buffer_short, f);

			// Ecriture du nom de l'adhérent
			buffer_short = strlen(member->forname) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->forname, sizeof(char), buffer_short, f);

			// Ecriture de l'adresse postale de l'adhérent
			fwrite(&member->adress->number, sizeof(unsigned short), 1, f);
			
			switch (member->adress->path)
			{
			case TRAIL:
				buffer_char = 0;
				break;

			case ROAD:
				buffer_char = 1;
				break;

			case DEADEND:
				buffer_char = 2;
				break;

			case STREET:
				buffer_char = 3;
				break;

			case AVENUE:
				buffer_char = 4;
				break;

			case BOULEVARD:
				buffer_char = 5;
				break;

			case PLACE:
				buffer_char = 6;
			}

			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			buffer_short = strlen(member->adress->pathname) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->adress->pathname, sizeof(char), buffer_short, f);

			fwrite(&member->adress->postal, sizeof(unsigned int), 1, f);

			buffer_short = strlen(member->adress->city) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->adress->city, sizeof(char), buffer_short, f);

			// Ecriture de l'adresse courriel de l'adhérent
			buffer_short = strlen(member->email->id) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->email->id, sizeof(char), buffer_short, f);

			buffer_short = strlen(member->email->hostname) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->email->hostname, sizeof(char), buffer_short, f);

			// Ecriture de la profession de l'adhérent
			buffer_short = strlen(member->profession) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->profession, sizeof(char), buffer_short, f);

			// Ecriture des emprunts
			fwrite(&member->n_borrows, sizeof(unsigned char), 1, f);
			
			if (member->n_borrows > 0)
			{
				for (j = 0 ; j < member->n_borrows ; j++)
				{
					buffer_char = strlen(member->borrows[j]) + 1;

					fwrite(&buffer_char, sizeof(unsigned char), 1, f);
					fwrite(member->borrows[j], sizeof(char), buffer_char, f);
				}
			}
		}
	}

	free(buffer_str);

	free(filename);
	fclose(f);

	return 1;
}

char member_freeDatabase(member_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	member_t *member = NULL;
	char *filename = (char*) malloc((8 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;
	unsigned char j = 0;

	// Tampons d'écriture
	char *buffer_str = NULL;
	unsigned short buffer_short = 0;
	unsigned char buffer_char = 0;

	if (db == NULL || filename == NULL)
		return 0;

	// Ouverture du fichier
	strcpy(filename, "db/");
	strcat(filename, db->name);
	strcat(filename, ".ldb");

	f = fopen(filename, "wb");
	if (f == NULL)
	{
		free(filename);

		return 0;
	}

	// Ecriture de l'entête
	buffer_str = (char*)malloc(9 * sizeof(char));
	if (buffer_str == NULL)
	{
		free(filename);
		fclose(f);

		return 0;
	}

	strcpy(buffer_str, LARBUNTU);
	fwrite(buffer_str, sizeof(char), 9, f);

	free(buffer_str);
	buffer_str = NULL;

	buffer_char = MEMBER;
	fwrite(&buffer_char, sizeof(unsigned char), 1, f);

	// Ecriture des informations sur la taille de base de données
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);
	fwrite(&db->deleted, sizeof(unsigned int), 1, f);

	for (i = 0; i < db->size; i++)
	{
		member = db->members[i];

		if (member == NULL)
		{
			// Lorsque l'entrée est vide
			buffer_char = BAD;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
		}
		else
		{
			// Lorsque l'entrée n'est pas vide
			buffer_char = VALID;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			// Ecriture de l'entrée dans la base de données
			fwrite(&member->id, sizeof(unsigned int), 1, f);

			// Ecriture du prénom de l'adhérent
			buffer_short = strlen(member->name) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->name, sizeof(char), buffer_short, f);

			// Ecriture du nom de l'adhérent
			buffer_short = strlen(member->forname) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->forname, sizeof(char), buffer_short, f);

			// Ecriture de l'adresse postale de l'adhérent
			fwrite(&member->adress->number, sizeof(unsigned short), 1, f);

			switch (member->adress->path)
			{
			case TRAIL:
				buffer_char = 0;
				break;

			case ROAD:
				buffer_char = 1;
				break;

			case DEADEND:
				buffer_char = 2;
				break;

			case STREET:
				buffer_char = 3;
				break;

			case AVENUE:
				buffer_char = 4;
				break;

			case BOULEVARD:
				buffer_char = 5;
				break;

			case PLACE:
				buffer_char = 6;
			}

			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			buffer_short = strlen(member->adress->pathname) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->adress->pathname, sizeof(char), buffer_short, f);

			fwrite(&member->adress->postal, sizeof(unsigned int), 1, f);

			buffer_short = strlen(member->adress->city) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->adress->city, sizeof(char), buffer_short, f);

			// Ecriture de l'adresse courriel de l'adhérent
			buffer_short = strlen(member->email->id) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->email->id, sizeof(char), buffer_short, f);

			buffer_short = strlen(member->email->hostname) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->email->hostname, sizeof(char), buffer_short, f);

			// Ecriture de la profession de l'adhérent
			buffer_short = strlen(member->profession) + 1;
			fwrite(&buffer_short, sizeof(unsigned short), 1, f);
			fwrite(member->profession, sizeof(char), buffer_short, f);

			// Ecriture des emprunts
			fwrite(&member->n_borrows, sizeof(unsigned char), 1, f);

			if (member->n_borrows > 0)
			{
				for (j = 0; j < member->n_borrows; j++)
				{
					buffer_char = strlen(member->borrows[j]) + 1;

					fwrite(&buffer_char, sizeof(unsigned char), 1, f);
					fwrite(member->borrows[j], sizeof(char), buffer_char, f);
				}
			}

			// Suppression des liens
			if (member->next != NULL)
				member->next->previous = NULL;

			member_free(member);
			db->members[i] = NULL;
		}
	}

	free(buffer_str);

	free(filename);
	fclose(f);

	free(db->members);
	free(db->name);
	free(db);

	return 1;
}

void member_displayDatabase(member_db *db)
{
	member_t *member = db->first;

	if (member == NULL)
		printf("Il n'y a aucun adherent enregistres dans la base de donnees.\n\n\n");
	else
	{
		do
		{

			printf("+============================================================\n");
			printf("| ID : %.10d\n", member->id);
			printf("| Nom : %s\n", member->name);
			printf("| Prenom : %s\n", member->forname);
			printf("| Profession : %s\n", member->profession);
			printf("| Adresse :\n");
			
			switch (member->adress->path)
			{
			case TRAIL:
				printf("| %d, chemin %s \n", member->adress->number, member->adress->pathname);
				break;
			case ROAD:
				printf("| %d, route %s \n", member->adress->number, member->adress->pathname);
				break;
			case DEADEND:
				printf("| %d, impasse %s \n", member->adress->number, member->adress->pathname);
				break;
			case STREET:
				printf("| %d, rue %s \n", member->adress->number, member->adress->pathname);
				break;
			case AVENUE:
				printf("| %d, avenue %s \n", member->adress->number, member->adress->pathname);
				break;
			case BOULEVARD:
				printf("| %d, boulevard %s \n", member->adress->number, member->adress->pathname);
				break;
			case PLACE:
				printf("| %d, place %s \n", member->adress->number, member->adress->pathname);
				break;
			}

			printf("| %.5d %s\n", member->adress->postal, member->adress->city);
			printf("| Email : %s@%s\n", member->email->id, member->email->hostname);
			printf("| Nombre de livres empruntes : %d\n", member->n_borrows);
			printf("| Codes des livres empruntes : ");
			if (member->borrows == NULL)
				printf("pas de livre empruntes\n");

			else
			{
				printf("%s\n", member->borrows);
			}
			

			member = member->next;
		} while (member != NULL);

		printf("+============================================================\n\n");
	}

	system("pause");
}

