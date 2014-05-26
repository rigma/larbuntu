#ifndef DB_H
#define DB_H

#include "thema.h"
#include "member.h"

/**
 * @struct db_t
 * @brief Représente une structure qui reroupe les différentes bases de données
 * @author Romain FAILLA
 */
typedef struct {
	member_db *db_member;
	thema_db *db_thema;
	book_db *db_book;
} db_t;

/**
 * @fn db_init(char *member, char *thema, char *book)
 * @brief Charge les tables de données du programme
 * @author Romain FAILLA
 * @param char *member : le nom de la base de données des membres
 * @param char *thema : le nom de la base de données des thèmes
 * @param char *book : le nom de la base de données des livres
 * @return db_t*
 */
db_t *db_init(char *member, char *thema, char *book);

/**
 * @fn db_save(db_t *db)
 * @brief Enregistre les bases de données du programme
 * @author Romain FAILLA
 * @param db_t *db : l'ensemble des tables
 * @return char
 */
char db_save(db_t *db);

/**
 * @fn db_free(db_t *db)
 * @brief Libère les tables de données du programme
 * @author Romain FAILLA
 * @param db_t *db : l'ensemble des tables
 */
char db_free(db_t *db);

#endif
