#ifndef MEMBER_H
#define MEMBER_H

#define MAX_BORROWS_MEMBER 3

/**
 * @struct member_t
 * @brief Repr�sente un adh�rent dans la base de donn�e
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct member_t {
	char *name;                // Le nom de l'adh�rent
	char *forname;             // Le pr�nom de l'adh�rent
	char *profession;          // La profession de l'adh�rent
	unsigned char n_borrows;   // Le nombre de livres emprunt�s
	char **borrows;            // Les codes des livres emprunt�s
	struct member_t *previous; // L'�l�ment pr�c�dent
	struct member_t *next;     // L'�l�ment suivant
} member_t;

/**
 * @struct member_db
 * @brief Repr�sente la base de donn�e des adh�rents
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct {
	char *name;
	unsigned int size;
	unsigned int next;
	member_t *first;
} member_db;

#endif
