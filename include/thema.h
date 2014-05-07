#ifndef THEMA_H
#define THEMA_H

#include "book.h"

#define MAX_BOOKS_THEMA 1000

/**
 * @struct thema_t
 * @brief Repr�sente un th�me de livre dans la base de donn�e
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct thema_t {
	unsigned int id;          // L'entr�e dans la base de donn�e
	char key[3];              // La cl� du th�me
	unsigned int size;        // Le nombre de livres dans le th�me (maximum 1000)
	book_t *books;            // Les livres du th�me
	struct thema_t *previous; // L'�l�ment pr�c�dent
	struct thema_t *next;     // L'�l�ment suivant
} thema_t;

/**
 * @struct thema_db
 * @brief Repr�sente la base de donn�e des th�mes
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct {
	char *name;
	unsigned int size;
	unsigned int next;
	thema_t *first;
} thema_db;

#endif
