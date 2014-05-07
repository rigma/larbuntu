#ifndef MEMBER_H
#define MEMBER_H

#define MAX_BORROWS_MEMBER 3

/**
 * @struct member_t
 * @brief Représente un adhérent dans la base de donnée
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct member_t {
	char *name;                // Le nom de l'adhérent
	char *forname;             // Le prénom de l'adhérent
	char *profession;          // La profession de l'adhérent
	unsigned char n_borrows;   // Le nombre de livres empruntés
	char **borrows;            // Les codes des livres empruntés
	struct member_t *previous; // L'élément précédent
	struct member_t *next;     // L'élément suivant
} member_t;

/**
 * @struct member_db
 * @brief Représente la base de donnée des adhérents
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct {
	char *name;
	unsigned int size;
	unsigned int next;
	member_t *first;
} member_db;

#endif
