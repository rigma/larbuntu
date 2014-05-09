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
	unsigned int size;        // Le nombre de livres dans le thème (maximum 1000)
	book_t *books;            // Les livres du thème
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
	thema_t *first;
	thema_t **themas;
} thema_db;

/**
 * @fn thema_init()
 * @brief Initialise une structure thema_t vide en mémoire
 * @author Romain FAILLA
 * @param char *key : la clé d'identification du thème
 * @return thema_t*
 */
thema_t *thema_init(char *key);

/**
 * @fn thema_free(thema_t *thema)
 * @brief Libère une structure thema_t de la mémoire
 * @author Romain FAILLA
 * @param thema_t *thema
 */
void thema_free(thema_t *thema);

#endif
