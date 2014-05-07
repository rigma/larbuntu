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
	unsigned int effective;  // Le nombre d'exemplaires possédés
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

/**
 * @fn book_init()
 * @brief Initialise une structure book_t vide en mémoire
 * @author Romain FAILLA
 * @return book_t*
 */
book_t *book_init();

/**
 * @fn book_free(book_t *book)
 * @brief Libére la structure désignée de la mémoire
 * @author Romain FAILLA
 * @param book_t *book : la structure à libérer
 */
void book_free(book_t *book);

#endif
