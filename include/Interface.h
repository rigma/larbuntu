#ifndef INTERFACE_H
#define INTERFACE_H

#include "db.h"

/**
* @fn interface_Capture()
* @brief Saisie dans le menu Interface
* @author Olivier NISOLE
* @param db_t *db : l'ensemble des tables de données
*/
void interface_Capture(db_t *db);

/**
* @fn interface_Delete()
* @brief Supression d'un element
* @author Olivier NISOLE
* @param db_t *db : l'ensemble des données
*/
void interface_Delete(db_t *db);

/**
* @fn interface_Borrow()
* @brief gestion emprunt des livres
* @author Olivier NISOLE
* @param db_t *db
*/
void interface_Borrow(db_t *db);
#endif