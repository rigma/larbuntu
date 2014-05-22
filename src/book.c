#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <book.h>
#include <dbio.h>

book_t *book_init()
{
	book_t *book = (book_t*) malloc(sizeof(book_t));
	if (book == NULL)
		return NULL;

	book->title = NULL;
	book->author = NULL;
	book->entry = 0;
	book->effective = 0;
	book->free = 0;
	book->d_borrows = NULL;
	book->previous = NULL;
	book->next = NULL;

	return book;
}

void book_free(book_t *book)
{
	free(book->title);
	free(book->author);
	free(book->d_borrows);
	free(book);
}

char book_add(book_db *db, book_t *book)
{
	book_t *previous = db->books[db->next - 1], *next = db->books[db->next + 1];
	book_t **tmp = NULL;
	unsigned int i = 0;

	if (db == NULL || book == NULL)
		return 0;

	if (previous != NULL)
	{
		previous->next = book;
		book->previous = previous;
	}

	if (next != NULL)
	{
		next->previous = book;
		book->next = next;
	}

	book->id = db->next;

	if (db->deleted == 0)
	{
		tmp = db->books;

		db->books = (book_t**) realloc(db->books, db->size++ * sizeof(book_t*));

		if (db->books == NULL)
		{
			db->books = tmp;

			return 0;
		}

		db->books[db->next] = book;
		db->next = db->size;
	}
	else
	{
		db->books[db->next] = book;
		db->deleted--;

		for (i = 0 ; i < db->size ; i++)
		{
			if (db->books[i] == NULL)
			{
				db->next = i;

				break;
			}
		}
	}

	return 1;
}

char book_remove(book_db *db, unsigned int index)
{
	book_t *book = NULL, *previous = NULL, *next = NULL;

	if (db == NULL || index >= db->size)
		return 0;

	book = db->books[index];

	if (book == NULL)
		return 0;
	else
	{
		previous = book->previous;
		next = book->next;

		previous->next = next;
		next->previous = previous;

		db->books[index] = NULL;
		book_free(book);

		db->next = index;
		db->deleted++;
	}

	return 1;
}

book_db *book_initDatabase(char *name)
{
	// Variables de travail
	FILE *f = NULL;
	book_db *db = (book_db*) malloc(sizeof(book_db));
	book_t *book = NULL, *previous =  NULL;
	char *filename = (char*) malloc((strlen(name) + 7) * sizeof(char));
	unsigned int i = 0;
	
	// Tampons de lecture
	char *buffer_str = NULL;
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
		db->deleted = 0;
		db->first = NULL;
		db->books = NULL;
	}
	else
	{
		// On v�rifie que la base de donn�e soit bien valide
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

		if (strcmp(buffer_str, LARBUNTU) || buffer_char != BOOK)
		{
			free(buffer_str);
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}
		
		free(buffer_str);

		// Lecture des entr�es
		strcpy(db->name, name);
		fread(&db->size, sizeof(unsigned int), 1, f);
		fread(&db->next, sizeof(unsigned int), 1, f);
		fread(&db->deleted, sizeof(unsigned int), 1, f);

		// Cr�ation du tableau contenant les pointeurs directs sur les livres
		db->books = (book_t**) malloc(db->size * sizeof(book_t*));
		if (db->books == NULL)
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
				db->books[i] = NULL;
		}

		for (i = 0 ; i < db->size ; i++)
		{
			// Lecture si l'entr�e n'est pas valide
			fread(&buffer_char, sizeof(unsigned char), 1, f);
			if (buffer_char == VALID)
			{
				book = book_init();

				// Lecture de l'identifiant
				fread(&book->id, sizeof(unsigned int), 1, f);

				// Lecture du titre
				fread(&buffer_char, sizeof(char), 1, f);

				book->title = (char*)malloc(buffer_char * sizeof(char));
				if (book->title == NULL)
				{
					book_free(book);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				fread(book->title, sizeof(char), buffer_char, f);

				// Lecture du nom de l'auteur
				fread(&buffer_char, sizeof(char), 1, f);

				book->author = (char*)malloc(buffer_char * sizeof(char));
				if (book->author == NULL)
				{
					book_free(book);
					free(db);

					free(filename);
					fclose(f);

					return NULL;
				}

				// Lecture de l'entr�e dans le th�me du livre
				fread(&book->entry, sizeof(short), 1, f);

				// Lecture du nombre d'exemplaires du livre poss�d�s
				fread(&book->effective, sizeof(unsigned int), 1, f);

				// Lecture du nombre d'exemplaires du livre poss�d�s
				fread(&book->free, sizeof(unsigned int), 1, f);

				// Lecture des dur�es d'emprunt
				book->d_borrows = (unsigned int*) malloc(book->effective * sizeof(unsigned int));
				if (book->d_borrows == NULL)
				{
					book_free(book);
					free(db);

					return NULL;
				}

				fread(book->d_borrows, sizeof(unsigned int), book->effective, f);

				// Cr�ation des liens de la cha�ne
				if (i == 0)
					db->first = book;
				else
				{
					previous->next = book;
					book->previous = previous;
				}

				db->books[book->id] = book;
				previous = book;
			}
		}
	}

	free(filename);
	fclose(f);

	return db;
}

char book_saveDatabase(book_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	book_t *book = NULL;
	char *filename = (char*) malloc((7 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;

	// Tampons d'�criture
	char *buffer_str = NULL;
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

	buffer_char = BOOK;
	fwrite(&buffer_char, sizeof(char), 1, f);

	// Ecriture des informations sur la taille de base de donn�es
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);
	fwrite(&db->deleted, sizeof(unsigned int), 1, f);

	// Ecriture des entr�es de la base de donn�es
	for (i = 0 ; i < db->size ; i++)
	{
		book = db->books[i];

		if (book == NULL)
		{
			// Lorsque l'entr�e est nulle, on pr�cise que l'entr�e n'est pas valide
			buffer_char = BAD;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
		}
		else
		{
			// Ecriture de l'entr�e valide
			buffer_char = VALID;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			// Ecriture de l'entr�e dans la base de donn�es
			fwrite(&book->id, sizeof(unsigned int), 1, f);

			// Ecriture du titre du livre
			buffer_char = strlen(book->title) + 1;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
			fwrite(book->title, sizeof(char), buffer_char + 1, f);

			// Ecriture de l'auteur du livre
			buffer_char = strlen(book->author) + 1;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
			fwrite(book->author, sizeof(char), buffer_char + 1, f);

			// Ecriture de l'entr�e du livre dans le th�me
			fwrite(&book->entry, sizeof(unsigned short), 1, f);

			// Ecriture de l'entr�e du nombre de livres poss�d�s
			fwrite(&book->effective, sizeof(unsigned int), 1, f);

			// Ecriture de l'entr�e du nombre de livres disponibles
			fwrite(&book->free, sizeof(unsigned int), 1, f);

			// Ecriture des dur�es d'emprunt
			fwrite(book->d_borrows, sizeof(unsigned int), book->effective, f);
		}
	}

	free(filename);
	fclose(f);

	return 1;
}

char book_freeDatabase(book_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	book_t *book = NULL;
	char *filename = (char*) malloc((7 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;

	// Tampons d'�criture
	char *buffer_str = NULL;
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

	buffer_char = BOOK;
	fwrite(&buffer_char, sizeof(char), 1, f);

	// Ecriture des informations sur la taille de la base de donn�es
	fwrite(&db->size, sizeof(unsigned int), 1, f);
	fwrite(&db->next, sizeof(unsigned int), 1, f);
	fwrite(&db->deleted, sizeof(unsigned int), 1, f);

	// Ecriture des entr�es de la base de donn�es
	for (i = 0 ; i < db->size ; i++)
	{
		book = db->books[i];

		if (book == NULL)
		{
			// Quand l'entr�e est nulle, on pr�cise que l'entr�e est fausse
			buffer_char = BAD;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
		}
		else
		{
			// Ecriture de l'entr�e valide
			buffer_char = VALID;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);

			// Ecriture de l'entr�e dans la base de donn�es
			fwrite(&book->id, sizeof(unsigned int), 1, f);

			// Ecriture du titre du livre
			buffer_char = strlen(book->title) + 1;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
			fwrite(book->title, sizeof(char), buffer_char + 1, f);

			// Ecriture de l'auteur du livre
			buffer_char = strlen(book->author) + 1;
			fwrite(&buffer_char, sizeof(unsigned char), 1, f);
			fwrite(book->author, sizeof(char), buffer_char + 1, f);

			// Ecriture de l'entr�e du livre dans le th�me
			fwrite(&book->entry, sizeof(unsigned short), 1, f);

			// Ecriture de l'entr�e du nombre de livres poss�d�s
			fwrite(&book->effective, sizeof(unsigned int), 1, f);

			// Ecriture de l'entr�e du nombre de livres disponibles
			fwrite(&book->free, sizeof(unsigned int), 1, f);

			// Ecriture des dur�es d'emprunt
			fwrite(book->d_borrows, sizeof(unsigned int), book->effective, f);

			// Lib�ration de l'�l�ment de la m�moire
			book->next->previous = NULL;

			book_free(book);
			db->books[i] = NULL;
		}
	}

	free(filename);
	fclose(f);

	free(db->name);
	free(db->books);
	free(db);

	return 1;
}
