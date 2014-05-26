#ifndef DB_H
#define DB_H

#include "thema.h"
#include "member.h"

/**
 * @struct db_t
 * @brief Repr�sente une structure qui reroupe les diff�rentes bases de donn�es
 * @author Romain FAILLA
 */
typedef struct {
	member_db *db_member;
	thema_db *db_thema;
	book_db *db_book;
} db_t;

/**
 * @fn db_init(char *member, char *thema, char *book)
 * @brief Charge les tables de donn�es du programme
 * @author Romain FAILLA
 * @param char *member : le nom de la base de donn�es des membres
 * @param char *thema : le nom de la base de donn�es des th�mes
 * @param char *book : le nom de la base de donn�es des livres
 * @return db_t*
 */
db_t *db_init(char *member, char *thema, char *book);

/**
 * @fn db_save(db_t *db)
 * @brief Enregistre les bases de donn�es du programme
 * @author Romain FAILLA
 * @param db_t *db : l'ensemble des tables
 * @return char
 */
char db_save(db_t *db);

/**
 * @fn db_free(db_t *db)
 * @brief Lib�re les tables de donn�es du programme
 * @author Romain FAILLA
 * @param db_t *db : l'ensemble des tables
 */
char db_free(db_t *db);

#endif
