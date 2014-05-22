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
	char key[4];              // La cl� du th�me
	unsigned short size;        // Le nombre de livres dans le th�me (maximum 1000)
	book_t **books;            // Les livres du th�me
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
	unsigned int deleted;
	thema_t *first;
	thema_t **themas;
} thema_db;

/**
 * @fn thema_init()
 * @brief Initialise une structure thema_t vide en m�moire
 * @author Romain FAILLA
 * @return thema_t*
 */
thema_t *thema_init();

/**
 * @fn thema_free(thema_t *thema)
 * @brief Lib�re une structure thema_t de la m�moire
 * @author Romain FAILLA
 * @param thema_t *thema
 */
void thema_free(thema_t *thema);

/**
 * @fn thema_add(thema_db *db, thema_t *thema)
 * @brief Ajoute un th�me dans la base de donn�es des th�mes
 * @author Romain FAILLA
 * @param thema_db *db : la base de donn�es des th�mes
 * @param thema_t *thema : le th�me � ajouter
 * @return char
 */
char thema_add(thema_db *db, thema_t *thema);

/**
 * @fn thema_remove(thema_db *db, unsigned int index)
 * @brief Retire un th�me de la base de donn�es des th�mes
 * @author Romain FAILLA
 * @param thema_db *db : la base de donn�es des th�mes
 * @param unsigned int index : l'index � enlever de la base de donn�es
 * @return char
 */
char thema_remove(thema_db *db, unsigned int index);

/**
 * @fn thema_initDatabase(book_db *db_books, char *name)
 * @brief Initialise la base de donn�es des th�mes
 * @author Romain FAILLA
 * @param book_db *db_books : la base de donn�es des livres
 * @param char *name : le nom de la base de donn�es
 * @return thema_db*
 */
thema_db *thema_initDatabase(book_db *db_books, char *name);

/**
 * @fn thema_saveDatabase(thema_db *db)
 * @brief Sauvegarde une base de donn�es des th�mes dans un fichier binaire
 * @author Romain FAILLA
 * @param thema_db *db : la base de donn�es � sauvegarder
 * @return char
 */
char thema_saveDatabase(thema_db *db);

/**
 * @fn thema_freeDatabase(thema_db *db)
 * @brief Lib�re la base de donn�es des th�mes de la m�moire et l'enregistre dans un fichier binaire
 * @author Romain FAILLA
 * @param thema_db *db : la base de donn�es � lib�rer
 * @return char
 */
char thema_freeDatabase(thema_db *db);

#endif
