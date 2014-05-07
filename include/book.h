#ifndef BOOK_H
#define BOOK_H

/**
 * @struct book_t
 * @brief Représente un livre dans la base de donnée
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct book_t {
	unsigned int id;         // L'entrée dans la base de donnée
	char *title;             // Le titre du livre
	char *author;            // L'auteur du livre
	unsigned short entry;    // L'entrée du livre dans un thème
	unsigned int effective;  // Le nombre d'exemplaires disponibles
	unsigned int free;       // Le nombre d'exemplaires libres
	unsigned int *d_borrows; // Les durées d'emprunt
	struct book_t *previous; // L'élément précédent
	struct book_t *next;     // L'élément suivant
} book_t;

/**
 * @struct book_db
 * @brief Représente la base de donnée des livres
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct {
	char *name;
	unsigned int size;
	unsigned int next;
	book_t *first;
} book_db;

#endif
