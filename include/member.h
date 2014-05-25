#ifndef MEMBER_H
#define MEMBER_H

#include "contact.h"

#define MAX_BORROWS_MEMBER 3
#define BORROWS_CODE 10

/**
 * @struct member_t
 * @brief Repr�sente un adh�rent dans la base de donn�e
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct member_t {
	unsigned int id;           // L'identifiant dans la base de donn�es
	char *name;                // Le nom de l'adh�rent
	char *forname;             // Le pr�nom de l'adh�rent
	adress_t *adress;          // L'adresse postale
	email_t *email;            // L'adresse courriel
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
	unsigned int deleted;
	member_t *first;
	member_t **members;
} member_db;

/**
 * @fn member_init()
 * @brief Initialise une structure member_t en m�moire
 * @author Olivier NISOLE
 * @return member_t *
 */
member_t *member_init();

/**
* @fn member_free()
* @brief Lib�re une structure member_t en m�moire
* @author Olivier NISOLE
* @param (member_t* member)
*/
void member_free(member_t *member);

/**
* @fn member_register()
* @brief Saisie les donn�es d'une variable de type member_t
* @author Olivier NISOLE
* @param (member_t* member)
* @return member_t*
*/
member_t* member_register(member_t *member);

/**
 * @fn member_initDatabase(char *name)
 * @brief Initialise la base de donn�es des membres
 * @author Romain FAILLA
 * @param char *name : le nom de la base de donn�es
 * @return member_db*
 */
member_db *member_initDatabase(char *name);

/**
 * @fn member_saveDatabase(member_db *db)
 * @brief Sauvegarde la base de donn�es des membres
 * @author Romain FAILLA
 * @param member_db *db : la base de donn�es des membres � sauvegarder
 * @return char
 */
char member_saveDatabase(member_db *db);

/**
 * @fn member_freeDatabase(member_db *db)
 * @brief Lib�re la base de donn�es des membres de la m�moire
 * @author Romain FAILLA
 * @param member_db *db : la base de donn�es des membres � lib�rer
 * @return char
 */
char member_freeDatabase(member_db *db);

#endif

