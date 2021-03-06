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
	thema_t *previous = NULL, *next = NULL, *current = NULL;
	thema_t **tmp = NULL;
	unsigned int i = 0;

	if (db == NULL || thema == NULL)
		return 0;

	previous = (db->next == 0) ? NULL : db->themas[db->next - 1];
	next = (db->next >= db->size) ? NULL : db->themas[db->next + 1];

	// On v�rifie que le th�me n'existe pas d�j�
	current = (db->first != NULL) ? db->first : NULL;
	while (current != NULL)
	{
		if (!strcmp(thema->key, current->key))
		{
			printf("Theme deja present !\n");

			return 0;
		}

		current = current->next;
	}

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

	if (db->size == 0)
		db->first = thema;

	if (db->deleted == 0)
	{
		db->size++;
		tmp = (thema_t**) realloc(db->themas, db->size * sizeof(thema_t*));

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

		if (previous != NULL && next != NULL)
		{
			previous->next = next;
			next->previous = previous;
		}
		else if (next != NULL)
			next->previous = NULL;
		else if (previous != NULL)
			previous->next = NULL;

		db->themas[index] = NULL;
		thema_free(thema);

		db->next = index;
		db->deleted++;

		if (db->size - db->deleted == 0)
			db->first = NULL;
	}

	return 1;
}

thema_t *thema_register(thema_t *thema)
{
	thema_t *t = NULL;
	char tmp[200] = { 0 };
	char answer = 0;

	if (thema == NULL)
	{
		t = thema_init();

		// Saisi de la cl� du th�me
		fflush(stdin);
		printf("Saississez la cle du theme (3 caracteres maximum) : ");
		fgets(tmp, sizeof(tmp), stdin);

		tmp[strlen(tmp) - 1] = '\0';
		strcpy(t->key, tmp);
	}
	else
	{
		t = thema;

		// Affichage des informations courantes sur le th�me � modifier
		printf("Th�me %d :\n", t->id);
		printf("+========================================\n");
		printf("| - Cl� : %s\n", t->key);
		printf("+========================================\n\n\n");

		do
		{
			fflush(stdin);
			printf("Voulez-vous modifiez la cl� ? [o/N] : ");
		} while (!scanf("%c", &answer));

		if (answer == 'o' || answer == 'O')
		{
			// Saisi de la cl� du th�me
			printf("Saississez la cl� du th�me (3 caracteres maximum) : ");
			fgets(tmp, sizeof(tmp), stdin);

			tmp[strlen(tmp) - 1] = '\0';
			strcpy(t->key, tmp);
		}
	}

	return t;
}

char thema_insertBook(thema_t *thema, book_t *book)
{
	book_t **tmp = NULL;

	if (thema == NULL || book == NULL)
		return 0;

	if (thema->size == MAX_BOOKS_THEMA)
		return 0;

	thema->size++;
	tmp = (book_t**) realloc(thema->books, thema->size * sizeof(book_t*));

	if (tmp == NULL)
	{
		thema->size--;

		return 0;
	}
	
	thema->books = tmp;

	book->entry = thema->size - 1;
	book->thema = (void*) thema;

	thema->books[thema->size - 1] = book;

	return 1;
}

char thema_removeBook(thema_t *thema, book_t *book)
{
	book_t **tmp = NULL;
	unsigned short size = 0;
	unsigned short i = 0, j = 0;

	if (thema == NULL || book == NULL)
		return 0;

	if (thema->size == 0)
		return 0;

	size = thema->size - 1;
	tmp = (book_t**) malloc(size * sizeof(book_t*));
	
	if (tmp == NULL)
		return 0;

	thema->books[book->entry] = NULL;
	while (i < size)
	{
		if (thema->books[j] != NULL)
		{
			tmp[i] = thema->books[j];

			i++;
			j++;
		}
		else
			j++;
	}

	free(thema->books);

	thema->books = tmp;
	thema->size = size;

	return 1;
}

thema_db *thema_initDatabase(book_db *db_books, char *name)
{
	// Variables de travail
	FILE *f = NULL;
	thema_db *db = (thema_db*) malloc(sizeof(thema_db));
	thema_t *thema = NULL, *previous = NULL;
	char *filename = (char*) malloc((strlen(name) + 8) * sizeof(char));
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
		db->deleted = 0;
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
			printf("Base de donn�es non valide.\n");

			free(buffer_str);
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}

		free(buffer_str);
		buffer_str = NULL;

		// On cr�e les ent�tes des fonctions
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
			fread(&buffer_char, sizeof(unsigned char), 1, f);
			if (buffer_char == VALID)
			{
				thema = thema_init();

				// Lecture de l'identifiant
				fread(&thema->id, sizeof(unsigned int), 1, f);

				// Lecture de la cl� du th�me
				fread(&thema->key, sizeof(char), 4, f);

				// Lecture du nombre de livres dans le th�me
				fread(&thema->size, sizeof(unsigned short), 1, f);

				// Lecture des identifiants des livres pr�sents dans le th�me
				thema->books = (book_t**) malloc(thema->size * sizeof(book_t*));
				if (thema->books == NULL) // TODO : am�liorer la gestion du cas o� thema->books n'a pas pu �tre initialis�.
				{
					perror("malloc : thema->books n'a pu etre initialise correctement.\n");

					exit(EXIT_FAILURE);
				}

				for (j = 0 ; j < thema->size ; j++)
				{
					fread(&buffer_int, sizeof(unsigned int), 1, f);

					thema->books[j] = db_books->books[buffer_int];
					db_books->books[buffer_int]->thema = (void*) thema;
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
	}

	free(buffer_str);
	free(filename);

	if (f != NULL)
		fclose(f);

	return db;
}

char thema_saveDatabase(thema_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	thema_t *thema = NULL;
	char *filename = (char*)malloc((8 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;
	unsigned short j = 0;

	// Tampons d'�criture
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

	// Ecriture de l'ent�te
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

	// Ecriture des �l�ments d'ent�te de la base de donn�es
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);
	fwrite(&db->deleted, sizeof(unsigned int), 1, f);

	// Ecriture des �l�ments de la base de donn�es
	for (i = 0; i < db->size; i++)
	{
		thema = db->themas[i];

		if (thema == NULL)
		{
			// Quand l'entr�e est nulle, on pr�cise que l'entr�e est fausse
			buffer_char = BAD;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
		}
		else
		{
			// Indication que l'entr�e est valide
			buffer_char = VALID;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			// Ecriture de l'identifiant du th�me
			fwrite(&thema->id, sizeof(unsigned int), 1, f);

			// Ecriture de la cl� du th�me
			fwrite(thema->key, sizeof(char), 4, f);

			// Ecriture du nombre de livres enregistr�s dans le th�me
			fwrite(&thema->size, sizeof(unsigned short), 1, f);

			// Ecriture des identifiants enregistr�s dans le th�me
			buffer_int = (unsigned int*) malloc(thema->size * sizeof(unsigned int));
			// TODO : g�rer le cas o� buffer_int a comme adresse NULL

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
	char *filename = (char*) malloc((8 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;
	unsigned short j = 0;

	// Tampons d'�criture
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
	buffer_str = NULL;

	buffer_char = THEMA;
	fwrite(&buffer_char, sizeof(unsigned char), 1, f);

	// Ecriture des �l�ments d'ent�te de la base de donn�es
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);
	fwrite(&db->deleted, sizeof(unsigned int), 1, f);

	// Ecriture des �l�ments de la base de donn�es
	for (i = 0 ; i < db->size ; i++)
	{
		thema = db->themas[i];

		if (thema == NULL)
		{
			// Quand l'entr�e est nulle, on pr�cise que l'entr�e est fausse
			buffer_char = BAD;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
		}
		else
		{
			// Indication que l'entr�e est valide
			buffer_char = VALID;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			// Ecriture de l'identifiant du th�me
			fwrite(&thema->id, sizeof(unsigned int), 1, f);

			// Ecriture de la cl� du th�me
			fwrite(thema->key, sizeof(char), 4, f);

			// Ecriture du nombre de livres enregistr�s dans le th�me
			fwrite(&thema->size, sizeof(unsigned short), 1, f);

			// Ecriture des identifiants enregistr�s dans le th�me
			buffer_int = (unsigned int*) malloc(thema->size * sizeof(unsigned int));
			// TODO : g�rer le cas o� buffer_int a comme adresse NULL

			for (j = 0; j < thema->size; j++)
				buffer_int[j] = thema->books[j]->id;

			fwrite(buffer_int, sizeof(unsigned int), thema->size, f);

			// Lib�ration de l'�l�ment
			if (thema->next != NULL)
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

book_t *thema_searchBookByCode(thema_db *db, char *code)
{
	thema_t *thema = db->first;
	char key[4] = { code[0], code[1], code[2], '\0' }, entry[4] = { code[4], code[5], code[6], '\0' };
	unsigned short i = 0;

	do
	{
		if (!strcmp(key, thema->key))
		{
			if (atoi(entry) >= thema->size)
				return NULL;
			else
			{
				for (i = 0; i < thema->size; i++)
				{
					if (thema->books[i]->entry == atoi(entry))
						return thema->books[i];
				}

				return NULL;
			}
		}

		thema = thema->next;
	} while (thema != NULL);

	return NULL;
}
