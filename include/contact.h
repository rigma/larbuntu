#ifndef CONTACT_H
#define CONTACT_H

/**
 * @enum path_t
 * @brief Représente les différentes voies dans une ville
 * @author Romain FAILLA
 */
typedef enum {
	TRAIL,
	ROAD,
	STREET,
	AVENUE,
	BOULEVARD
} path_t;

/**
 * @struct adress_t
 * @brief Représente une adresse postale
 * @author Romain FAILLA
 */
typedef struct {
	unsigned short number;
	path_t path;
	char *pathname;
	unsigned int postal;
	char *city;
} adress_t;

/**
 * @struct email_t
 * @brief Représente une adresse courriel
 * @author Romain FAILLA
 */
typedef struct {
	char *id;
	char *hostname;
} email_t;

/**
 * @fn adress_init(unsigned short number, path_t path, char *pathname, unsigned int postal, char *city)
 * @brief Initialise une adresse postale
 * @author Romain FAILLA
 * @param unsigned short number : numéro de l'habitation
 * @param path_t path : indique le type de voie
 * @param char *pathname : indique le nom de la voie
 * @param unsigned int postal : indique le code postal de l'adresse
 * @param char *city : indique le nom de la ville
 * @return adress_t*
 */
adress_t *adress_init(unsigned short number, path_t path, char *pathname, unsigned int postal, char *city);

/**
 * @fn adress_free(adress_t *adress)
 * @brief Libère de la mémoire une adresse postale
 * @author Romain FAILLA
 * @param adress_t *adress : l'adresse postale à libérer
 */
void adress_free(adress_t *adress);

/**
 * @fn email_init(char *id, char *hostname)
 * @brief Initialise une adresse courriel
 * @author Romain FAILLA
 * @param char *id : indique l'identifiant du destinataire
 * @param char *hostname : indique le nom de domaine du serveur du destinaire
 * @return email_t*
 */
email_t *email_init(char *id, char *hostname);

/**
 * @fn email_free(email_t *email)
 * @brief Libère de la mémoire une adresse courriel
 * @author Romain FAILLA
 * @param email_t *email : l'adresse courriel à libérer
 */
void email_free(email_t *email);

/**
 * @fn email_register(email_t *email)
  * @brief Saisie de l'adresse e-mail de l'utilisateur
   * @author Olivier NISOLE
   * @param email_t *email
   */
email_t* email_register(email_t *email);

/**
* @fn adress_register(adress_t *adress)
* @brief Saisie de l'adresse de l'utilisateur
* @author olivier NISOLE
* @param adress_t *adress
*/
adress_t* adress_register(adress_t *adress);

#endif
