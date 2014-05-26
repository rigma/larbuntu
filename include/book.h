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
	unsigned int deleted;
	book_t *first;
	book_t **books;
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

/**
 * @fn book_add(book_db *db, book_t *book)
 * @brief Ajoute un livre dans la base de données des livres au prochain index libre
 * @author Romain FAILLA
 * @param book_db *db : la base de données des livres
 * @param book_t *book : le livre à ajouter
 * @return char
 */
char book_add(book_db *db, book_t *book);

/**
 * @fn book_remove(book_db *db, unsigned int id)
 * @brief Enlève le livre à l'index id de la base de données des livres
 * @author Romain FAILLA
 * @param book_db *db : la base de données des livres
 * @param unsigned int id : l'index à enlever dans la base de données
 * @return char
 */
char book_remove(book_db *db, unsigned int id);

/**
 * @fn book_initDatabase(char *name)
 * @brief Initialise la base de données des livres
 * @author Romain FAILLA
 * @param char *name : le nom de la base de donnée
 * @return book_db*
 */
book_db *book_initDatabase(char *name);

/**
 * @fn book_saveDatabase(book_db *db)
 * @brief Sauvegarde une base de données des livres dans un fichier binaire
 * @author Romain FAILLA
 * @param book_db *db : la base de données à sauvegarder
 * @return char
 */
char book_saveDatabase(book_db *db);

/**
 * @fn book_freeDatabase(book_db *db)
 * @brief Libère la base de données des livres de la mémoire et la sauvegarde sur un fichier binaire
 * @author Romain FAILLA
 * @param book_db *db : la base de données à libérer
 * @return char
 */
char book_freeDatabase(book_db *db);

/**
 * @fn book_register()
 * @brief Saisie d'un livre
 * @author Olivier NISOLE
 * @param  book_t* book
 * @return book_t*
 */
book_t* book_register(book_t* book);

#endif
