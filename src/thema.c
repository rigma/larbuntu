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
	if (thema == NULL)
	{
		free(thema->books);
		free(thema);
	}
}

char thema_add(thema_db *db, thema_t *thema)
{
	thema_t *previous = db->themas[db->next - 1], *next = db->themas[db->next + 1];
	thema_t **tmp = NULL;
	unsigned int i = 0;

	if (db == NULL || thema == NULL)
		return 0;

	if (previous != NULL)
	{
		previous->next = thema;
		thema->previous = previous;
	}

	if (next != NULL)
	{
		next->previous = thema;
		thema->next = next;
	}

	thema->id = db->next;

	if (db->deleted == 0)
	{
		db->size++;
		tmp = (thema_t**) realloc(db->themas, db->size++ * sizeof(thema_t*));

		if (tmp != NULL)
			db->themas = tmp;
		else
			return 0;

		db->themas[db->next] = thema;
		db->next = db->size;
	}
	else
	{
		db->themas[db->next] = thema;
		db->deleted--;

		for (i = 0; i < db->size; i++)
		{
			if (db->themas[i] == NULL)
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

char thema_remove(thema_db *db, unsigned int index)
{
	thema_t *thema = NULL, *previous = NULL, *next = NULL;

	if (db == NULL || index >= db->size)
		return 0;

	thema = db->themas[index];

	if (thema == NULL)
		return 0;
	else
	{
		previous = thema->previous;
		next = thema->next;

		previous->next = next;
		next->previous = previous;

		db->themas[index] = NULL;
		thema_free(thema);

		db->next = index;
		db->deleted++;
	}

	return 1;
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
	strcat(filename, ".ldb");

	f = fopen(filename, "rb");
	if (f == NULL)
	{
		// On initialise une base de données vide
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
		db->themas = NULL;
	}
	else
	{
		// On vérifie que la base de données est valide
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
			printf("Base de données non valide.\n");

			free(buffer_str);
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}

		free(buffer_str);
		buffer_str = NULL;

		// On crée les entêtes des fonctions
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

		// Créations du tableau contenant les adresses des éléments de la base de donnée
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
			fread(&buffer_char, sizeof(unsigned char), 1, f);
			if (buffer_char == VALID)
			{
				thema = thema_init();

				// Lecture de l'identifiant
				fread(&thema->id, sizeof(unsigned int), 1, f);

				// Lecture de la clé du thème
				fread(&thema->key, sizeof(char), 4, f);

				// Lecture du nombre de livres dans le thème
				fread(&thema->size, sizeof(unsigned short), 1, f);

				// Lecture des identifiants des livres présents dans le thème
				thema->books = (book_t**) malloc(thema->size * sizeof(book_t*));
				if (thema->books == NULL) // TODO : améliorer la gestion du cas où thema->books n'a pas pu être initialisé.
				{
					perror("malloc : thema->books n'a pu etre initialise correctement.\n");

					exit(EXIT_FAILURE);
				}

				for (j = 0 ; j < thema->size ; j++)
				{
					fread(&buffer_int, sizeof(unsigned int), 1, f);
					thema->books[j] = db_books->books[buffer_int];
				}

				// Création des liens de l'élément au sein de la liste
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
	}

	free(buffer_str);
	free(filename);

	if (f == NULL)
		fclose(f);

	return db;
}

char thema_saveDatabase(thema_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	thema_t *thema = NULL;
	char *filename = (char*)malloc((7 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;
	unsigned short j = 0;

	// Tampons d'écriture
	char *buffer_str = NULL;
	unsigned int *buffer_int = NULL;
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

	buffer_char = THEMA;
	fwrite(&buffer_char, sizeof(unsigned char), 1, f);

	// Ecriture des éléments d'entête de la base de données
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);
	fwrite(&db->deleted, sizeof(unsigned int), 1, f);

	// Ecriture des éléments de la base de données
	for (i = 0; i < db->size; i++)
	{
		thema = db->themas[i];

		if (thema == NULL)
		{
			// Quand l'entrée est nulle, on précise que l'entrée est fausse
			buffer_char = BAD;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
		}
		else
		{
			// Indication que l'entrée est valide
			buffer_char = VALID;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			// Ecriture de l'identifiant du thème
			fwrite(&thema->id, sizeof(unsigned int), 1, f);

			// Ecriture de la clé du thème
			fwrite(thema->key, sizeof(char), 4, f);

			// Ecriture du nombre de livres enregistrés dans le thème
			fwrite(&thema->size, sizeof(unsigned short), 1, f);

			// Ecriture des identifiants enregistrés dans le thème
			buffer_int = (unsigned int*) malloc(thema->size * sizeof(unsigned int));
			// TODO : gérer le cas où buffer_int a comme adresse NULL

			for (j = 0 ; j < thema->size ; j++)
				buffer_int[j] = thema->books[j]->id;

			fwrite(buffer_int, sizeof(unsigned int), thema->size, f);
		}
	}

	free(buffer_str);
	free(buffer_int);

	free(filename);
	fclose(f);

	return 1;
}

char thema_freeDatabase(thema_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	thema_t *thema = NULL;
	char *filename = (char*) malloc((7 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;
	unsigned short j = 0;

	// Tampons d'écriture
	char *buffer_str = NULL;
	unsigned int *buffer_int = NULL;
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
	buffer_str = NULL;

	buffer_char = THEMA;
	fwrite(&buffer_char, sizeof(unsigned char), 1, f);

	// Ecriture des éléments d'entête de la base de données
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);
	fwrite(&db->deleted, sizeof(unsigned int), 1, f);

	// Ecriture des éléments de la base de données
	for (i = 0 ; i < db->size ; i++)
	{
		thema = db->themas[i];

		if (thema == NULL)
		{
			// Quand l'entrée est nulle, on précise que l'entrée est fausse
			buffer_char = BAD;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
		}
		else
		{
			// Indication que l'entrée est valide
			buffer_char = VALID;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			// Ecriture de l'identifiant du thème
			fwrite(&thema->id, sizeof(unsigned int), 1, f);

			// Ecriture de la clé du thème
			fwrite(thema->key, sizeof(char), 4, f);

			// Ecriture du nombre de livres enregistrés dans le thème
			fwrite(&thema->size, sizeof(unsigned short), 1, f);

			// Ecriture des identifiants enregistrés dans le thème
			buffer_int = (unsigned int*) malloc(thema->size * sizeof(unsigned int));
			// TODO : gérer le cas où buffer_int a comme adresse NULL

			for (j = 0; j < thema->size; j++)
				buffer_int[j] = thema->books[j]->id;

			fwrite(buffer_int, sizeof(unsigned int), thema->size, f);

			// Libération de l'élément
			if (thema->next == NULL)
				thema->next->previous = NULL;

			thema_free(thema);
			db->themas[i] = NULL;
		}
	}

	free(buffer_str);
	free(filename);

	fclose(f);

	free(db->name);
	free(db->themas);
	free(db);

	return 1;
}
