#ifndef THEMA_H
#define THEMA_H

#include "book.h"

#define MAX_BOOKS_THEMA 1000

/**
 * @struct thema_t
 * @brief Représente un thème de livre dans la base de donnée
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct thema_t {
	unsigned int id;          // L'entrée dans la base de donnée
	char key[4];              // La clé du thème
	unsigned short size;        // Le nombre de livres dans le thème (maximum 1000)
	book_t **books;            // Les livres du thème
	struct thema_t *previous; // L'élément précédent
	struct thema_t *next;     // L'élément suivant
} thema_t;

/**
 * @struct thema_db
 * @brief Représente la base de donnée des thèmes
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
 * @brief Initialise une structure thema_t vide en mémoire
 * @author Romain FAILLA
 * @return thema_t*
 */
thema_t *thema_init();

/**
 * @fn thema_free(thema_t *thema)
 * @brief Libère une structure thema_t de la mémoire
 * @author Romain FAILLA
 * @param thema_t *thema
 */
void thema_free(thema_t *thema);

/**
 * @fn thema_add(thema_db *db, thema_t *thema)
 * @brief Ajoute un thème dans la base de données des thèmes
 * @author Romain FAILLA
 * @param thema_db *db : la base de données des thèmes
 * @param thema_t *thema : le thème à ajouter
 * @return char
 */
char thema_add(thema_db *db, thema_t *thema);

/**
 * @fn thema_remove(thema_db *db, unsigned int index)
 * @brief Retire un thème de la base de données des thèmes
 * @author Romain FAILLA
 * @param thema_db *db : la base de données des thèmes
 * @param unsigned int index : l'index à enlever de la base de données
 * @return char
 */
char thema_remove(thema_db *db, unsigned int index);

/**
 * @fn thema_initDatabase(book_db *db_books, char *name)
 * @brief Initialise la base de données des thèmes
 * @author Romain FAILLA
 * @param book_db *db_books : la base de données des livres
 * @param char *name : le nom de la base de données
 * @return thema_db*
 */
thema_db *thema_initDatabase(book_db *db_books, char *name);

/**
 * @fn thema_saveDatabase(thema_db *db)
 * @brief Sauvegarde une base de données des thèmes dans un fichier binaire
 * @author Romain FAILLA
 * @param thema_db *db : la base de données à sauvegarder
 * @return char
 */
char thema_saveDatabase(thema_db *db);

/**
 * @fn thema_freeDatabase(thema_db *db)
 * @brief Libère la base de données des thèmes de la mémoire et l'enregistre dans un fichier binaire
 * @author Romain FAILLA
 * @param thema_db *db : la base de données à libérer
 * @return char
 */
char thema_freeDatabase(thema_db *db);

#endif
