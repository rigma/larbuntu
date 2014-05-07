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
	unsigned char *buffer_str = NULL;
	unsigned char buffer_char = 0;
	unsigned short buffer_short = 0;
	unsigned int buffer_int = 0;

	if (db == NULL)
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

			return NULL;
		}

		fread(buffer_str, sizeof(char), 9, f);
		fread(&buffer_char, sizeof(char), 1, f);

		if (strcmp(buffer_str, LARBUNTU) || buffer_char != BOOK)
		{
			free(buffer_str);
			free(db);

			return NULL;
		}
		
		free(buffer_str);

		// Lecture des entrées
		fread(&db->size, sizeof(unsigned int), 1, f);
		fread(&db->next, sizeof(unsigned int), 1, f);

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

			previous = book;
		}
	}

	free(filename);
	fclose(f);

	return db;
}
