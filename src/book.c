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

book_db *book_initDatabase(char *name)
{
	// Variables de travail
	FILE *f = NULL;
	book_db *db = (book_db*) malloc(sizeof(book_db));
	book_t *book = NULL, *previous =  NULL;
	char *filename = (char*) malloc((7 + strlen(name)) * sizeof(char));
	unsigned int i = 0;
	
	// Tampons de lecture
	char *buffer_str = NULL;
	unsigned char buffer_char = 0;

	if (db == NULL || filename == NULL)
		return NULL;

	strcpy(filename, "db/");
	strcat(filename, name);
	strcat(filename, ".db");

	f = fopen(filename, "rb");
	if (f == NULL)
	{
		// On initialise une base de donnée vide
		strcpy(db->name, name);
		db->size = 0;
		db->next = 0;
		db->first = NULL;
	}
	else
	{
		// On vérifie que la base de donnée soit bien valide
		buffer_str = (char*) malloc(9 * sizeof(char));
		if (buffer_str == NULL)
		{
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}

		fread(buffer_str, sizeof(char), 9, f);
		fread(&buffer_char, sizeof(char), 1, f);

		if (strcmp(buffer_str, LARBUNTU) || buffer_char != BOOK)
		{
			free(buffer_str);
			free(db);

			free(filename);
			fclose(f);

			return NULL;
		}
		
		free(buffer_str);

		// Lecture des entrées
		strcpy(db->name, name);
		fread(&db->size, sizeof(unsigned int), 1, f);
		fread(&db->next, sizeof(unsigned int), 1, f);

		// Création du tableau contenant les pointeurs directs sur les livres
		db->books = (book_t**) malloc(db->size * sizeof(book_t*));
		if (db->books == NULL)
		{
			free(buffer_str);
			free(db->name);
			free(db);

			return NULL;
		}

		for (i = 0 ; i < db->size ; i++)
		{
			book = book_init();

			// Lecture de l'id
			fread(&book->id, sizeof(unsigned int), 1, f);
			
			// Lecture du titre
			fread(&buffer_char, sizeof(char), 1, f);

			book->title = (char*) malloc(buffer_char * sizeof(char));
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

			book->author = (char*) malloc(buffer_char * sizeof(char));
			if (book->author == NULL)
			{
				book_free(book);
				free(db);

				free(filename);
				fclose(f);

				return NULL;
			}

			// Lecture de l'entrée dans le thème du livre
			fread(&book->entry, sizeof(short), 1, f);

			// Lecture du nombre d'exemplaires du livre possédés
			fread(&book->effective, sizeof(unsigned int), 1, f);

			// Lecture du nombre d'exemplaires du livre possédés
			fread(&book->free, sizeof(unsigned int), 1, f);

			// Lecture des durées d'emprunt
			book->d_borrows = (unsigned int*) malloc(book->effective * sizeof(unsigned int));
			if (book->d_borrows == NULL)
			{
				book_free(book);
				free(db);

				return NULL;
			}

			fread(book->d_borrows, sizeof(unsigned int), book->effective, f);

			// Création des liens de la chaîne
			if (i == 0)
				db->first = book;
			else
			{
				previous->next = book;
				book->previous = previous;
			}

			db->books[i] = book;
			previous = book;
		}
	}

	free(filename);
	fclose(f);

	return db;
}

char book_freeDatabase(book_db *db)
{
	// Variables de travail
	FILE *f = NULL;
	book_t *book = NULL;
	char *filename = (char*) malloc((7 + strlen(db->name)) * sizeof(char));
	unsigned int i = 0;

	// Tampon d'écriture
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
		return 0;

	// Ecriture de l'entête
	buffer_str = (char*) malloc(9 * sizeof(char));
	if (buffer_str == NULL)
	{
		fclose(f);

		return 0;
	}

	fwrite(buffer_str, sizeof(char), 9, f);

	buffer_char = BOOK;
	fwrite(&buffer_char, sizeof(char), 1, f);

	// Ecriture des entrées de la base de données
	for (i = 0 ; i < db->size ; i++)
	{
		book = db->books[i];

		// Ecriture de l'entrée dans la base de données
		fwrite(&book->id, sizeof(unsigned int), 1, f);

		// Ecriture du titre du livre
		buffer_char = strlen(book->title) + 1;
		fwrite(&buffer_char, sizeof(char), 1, f);
		fwrite(book->title, sizeof(char), buffer_char + 1, f);

		// Ecriture de l'auteur du livre
		buffer_char = strlen(book->author) + 1;
		fwrite(&buffer_char, sizeof(char), 1, f);
		fwrite(book->author, sizeof(char), buffer_char + 1, f);

		// Ecriture de l'entrée du livre dans le thème
		fwrite(&book->entry, sizeof(unsigned short), 1, f);

		// Ecriture de l'entrée du nombre de livres possédés
		fwrite(&book->effective, sizeof(unsigned int), 1, f);

		// Ecriture de l'entrée du nombre de livres disponibles
		fwrite(&book->free, sizeof(unsigned int), 1, f);

		// Ecriture des durées d'emprunt
		fwrite(book->d_borrows, sizeof(unsigned int), book->effective, f);

		// Libération de l'élément de la mémoire
		book->next->previous = NULL;

		book_free(book);
		db->books[i] = NULL;
	}

	fclose(f);

	free(db->name);
	free(db->books);
	free(db);

	return 1;
}
