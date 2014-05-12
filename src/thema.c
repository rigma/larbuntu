#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dbio.h>
#include <thema.h>

thema_t *thema_init()
{
	thema_t *thema = (thema_t*) malloc(sizeof(thema_t));
	if (thema == NULL)
		return NULL;

	thema->id = 0;
	thema->size = 0;
	thema->books = NULL;
	thema->previous = NULL;
	thema->next = NULL;

	return thema;
}

void thema_free(thema_t *thema)
{
	free(thema->books);
	free(thema);
}

thema_db *thema_initDatabase(book_db *db_books, char *name)
{
	// Variables de travail
	FILE *f = NULL;
	thema_db *db = (thema_db*) malloc(sizeof(thema_db));
	thema_t *thema = NULL, *previous = NULL;
	char *filename = (char*) malloc((strlen(name) + 7) * sizeof(char));
	unsigned int i = 0;
	unsigned short j = 0;

	// Tampons de lecture
	char *buffer_str = NULL;
	unsigned char buffer_char = 0;
	unsigned int buffer_int = 0;

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
	strcat(filename, ".db");

	f = fopen(filename, "rb");
	if (f == NULL)
	{
		// On initialise une base de donn�es vide
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
		db->first = NULL;
		db->themas = NULL;
	}
	else
	{
		// On v�rifie que la base de donn�es est valide
		buffer_str = (char*) malloc(9 * sizeof(char));
		if (buffer_str == NULL)
		{
			free(db);
			free(filename);

			return NULL;
		}

		fread(buffer_str, sizeof(char), 9, f);
		fread(&buffer_char, sizeof(char), 1, f);

		if (strcmp(buffer_str, LARBUNTU) || buffer_char != THEMA)
		{
			free(buffer_str);
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}

		free(buffer_str);

		// On cr�e les ent�tes des fonctions
		strcpy(db->name, name);
		fread(&db->size, sizeof(unsigned int), 1, f);
		fread(&db->next, sizeof(unsigned int), 1, f);

		// Cr�ations du tableau contenant les adresses des �l�ments de la base de donn�e
		db->themas = (thema_t**) malloc(db->size * sizeof(thema_t*));
		if (db->themas == NULL)
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
				db->themas[i] = NULL;
		}

		for (i = 0 ; i < db->size ; i++)
		{
			thema = thema_init();
			
			// Lecture de l'identifiant
			fread(&thema->id, sizeof(unsigned int), 1, f);

			// Lecture de la cl� du th�me
			fread(&thema->key, sizeof(char), 4, f);

			// Lecture du nombre de livres dans le th�me
			fread(&thema->size, sizeof(unsigned short), 1, f);

			// Lecture des identifiants des livres pr�sents dans le th�me
			thema->books = (book_t**)malloc(thema->size * sizeof(book_t*));
			if (thema->books == NULL) // TODO : am�liorer la gestion du cas o� thema->books n'a pas pu �tre initialis�.
			{
				perror("malloc : thema->books n'a pu etre initialise correctement.\n");

				exit(EXIT_FAILURE);
			}

			for (j = 0 ; j < thema->size ; j++)
			{
				fread(&buffer_int, sizeof(unsigned int), 1, f);
				thema->books[j] = db_books->books[buffer_int];
			}

			// Cr�ation des liens de l'�l�ment au sein de la liste
			if (i == 0)
				db->first = thema;
			else
			{
				previous->next = thema;
				thema->previous = previous;
			}

			db->themas[thema->id] = thema;
			previous = thema;
		}
	}

	free(filename);
	fclose(f);

	return db;
}

char thema_freeDatabase(thema_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	thema_t *thema = NULL;
	char *filename = (char*) malloc((7 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;
	unsigned short j = 0;

	// Tampons d'�criture
	char *buffer_str = NULL;
	int *buffer_int = NULL;
	char buffer_char = 0;

	if (db == NULL || filename == NULL)
		return 0;

	// Ouverture du fichier
	strcpy(filename, "db/");
	strcat(filename, db->name);
	strcat(filename, ".db");

	f = fopen(filename, "wb");
	if (f == NULL)
	{
		free(filename);

		return 0;
	}

	// Ecriture de l'ent�te
	buffer_str = (char*) malloc(9 * sizeof(char));
	if (buffer_str == NULL)
	{
		free(filename);
		fclose(f);

		return 0;
	}

	strcpy(buffer_str, LARBUNTU);
	fwrite(buffer_str, sizeof(char), 9, f);
	free(buffer_str);

	buffer_char = THEMA;
	fwrite(&buffer_char, sizeof(char), 1, f);

	// Ecriture des �l�ments d'ent�te de la base de donn�es
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);

	// Ecriture des �l�ments de la base de donn�es
	for (i = 0 ; i < db->size ; i++)
	{
		thema = db->themas[i];

		// Ecriture de l'identifiant du th�me
		fwrite(&thema->id, sizeof(int), 1, f);

		// Ecriture de la cl� du th�me
		fwrite(thema->key, sizeof(char), 4, f);

		// Ecriture du nombre de livres enregistr�s dans le th�me
		fwrite(&thema->size, sizeof(unsigned short), 1, f);

		// Ecriture des identifiants enregistr�s dans le th�me
		buffer_int = (int*) malloc(thema->size * sizeof(int));
		// TODO : g�rer le cas o� buffer_int a comme adresse NULL

		for (j = 0 ; j < thema->size ; j++)
			buffer_int[j] = thema->books[j]->id;

		fwrite(buffer_int, sizeof(int), thema->size, f);

		// Lib�ration de l'�l�ment
		thema->next->previous = NULL;

		thema_free(thema);
		db->themas[i] = NULL;
	}

	fclose(f);

	free(db->name);
	free(db->themas);
	free(db);

	return 1;
}