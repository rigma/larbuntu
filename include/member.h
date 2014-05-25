#ifndef MEMBER_H
#define MEMBER_H

#include "contact.h"

#define MAX_BORROWS_MEMBER 3
#define BORROWS_CODE 10

/**
 * @struct member_t
 * @brief Représente un adhérent dans la base de donnée
 * @author Romain FAILLA et Olivier NISOLE
 */
typedef struct member_t {
	unsigned int id;           // L'identifiant dans la base de données
	char *name;                // Le nom de l'adhérent
	char *forname;             // Le prénom de l'adhérent
	adress_t *adress;          // L'adresse postale
	email_t *email;            // L'adresse courriel
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
	unsigned int deleted;
	member_t *first;
	member_t **members;
} member_db;

/**
 * @fn member_init()
 * @brief Initialise une structure member_t en mémoire
 * @author Olivier NISOLE
 * @return member_t *
 */
member_t *member_init();

/**
* @fn member_free()
* @brief Libère une structure member_t en mémoire
* @author Olivier NISOLE
* @param (member_t* member)
*/
void member_free(member_t *member);

/**
* @fn member_register()
* @brief Saisie les données d'une variable de type member_t
* @author Olivier NISOLE
* @param (member_t* member)
* @return member_t*
*/
member_t* member_register(member_t *member);

/**
 * @fn member_initDatabase(char *name)
 * @brief Initialise la base de données des membres
 * @author Romain FAILLA
 * @param char *name : le nom de la base de données
 * @return member_db*
 */
member_db *member_initDatabase(char *name);

/**
 * @fn member_saveDatabase(member_db *db)
 * @brief Sauvegarde la base de données des membres
 * @author Romain FAILLA
 * @param member_db *db : la base de données des membres à sauvegarder
 * @return char
 */
char member_saveDatabase(member_db *db);

/**
 * @fn member_freeDatabase(member_db *db)
 * @brief Libère la base de données des membres de la mémoire
 * @author Romain FAILLA
 * @param member_db *db : la base de données des membres à libérer
 * @return char
 */
char member_freeDatabase(member_db *db);

#endif

