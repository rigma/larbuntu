#ifndef BOOK_H
#define BOOK_H

/**
 * @struct book_t
 * @brief Repr�sente un livre dans la base de donn�e
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct book_t {
	unsigned int id;         // L'entr�e dans la base de donn�e
	char *title;             // Le titre du livre
	char *author;            // L'auteur du livre
	unsigned short entry;    // L'entr�e du livre dans un th�me
	unsigned int effective;  // Le nombre d'exemplaires poss�d�s
	unsigned int free;       // Le nombre d'exemplaires libres
	unsigned int *d_borrows; // Les dur�es d'emprunt
	struct book_t *previous; // L'�l�ment pr�c�dent
	struct book_t *next;     // L'�l�ment suivant
} book_t;

/**
 * @struct book_db
 * @brief Repr�sente la base de donn�e des livres
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
 * @brief Initialise une structure book_t vide en m�moire
 * @author Romain FAILLA
 * @return book_t*
 */
book_t *book_init();

/**
 * @fn book_free(book_t *book)
 * @brief Lib�re la structure d�sign�e de la m�moire
 * @author Romain FAILLA
 * @param book_t *book : la structure � lib�rer
 */
void book_free(book_t *book);

#endif
