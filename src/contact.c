#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <contact.h>

adress_t *adress_init(unsigned short number, path_t path, char *pathname, unsigned int postal, char *city)
{
	adress_t *adress = (adress_t*) malloc(sizeof(adress_t));
	if (adress == NULL)
		return NULL;

	adress->number = number;
	adress->path = path;

	adress->pathname = (char*) malloc((1 + strlen(pathname)) * sizeof(char));
	if (adress->pathname == NULL)
	{
		free(adress);

		return NULL;
	}

	strcpy(adress->pathname, pathname);

	adress->postal = postal;

	adress->city = (char*) malloc((1 + strlen(city)) * sizeof(char));
	if (adress->city == NULL)
	{
		free(adress->pathname);
		free(adress);

		return NULL;
	}

	strcpy(adress->city, city);

	return adress;
}

void adress_free(adress_t *adress)
{
	free(adress->pathname);
	free(adress->city);
	free(adress);
}

email_t *email_init(char *id, char *hostname)
{
	email_t *email = (email_t*) malloc(sizeof(email_t));
	if (email == NULL)
		return NULL;

	email->id = (char*) malloc((1 + strlen(id)) * sizeof(char));
	if (email->id == NULL)
	{
		free(email);

		return NULL;
	}

	strcpy(email->id, id);

	email->hostname = (char*) malloc((1 + strlen(hostname)) * sizeof(char));
	if (email->hostname == NULL)
	{
		free(email->id);
		free(email);

		return NULL;
	}

	strcpy(email->hostname, hostname);

	return email;
}

void email_free(email_t *email)
{
	free(email->id);
	free(email->hostname);
	free(email);
}

email_t* email_register(email_t *email)
{
	email_t *e = NULL;
	char id[200], hostname[200];

	printf("\n --- Saisi de l'adresse courriel ---\n");
	if (email == NULL)
	{
		fflush(stdin);
		printf("Veuillez entrer l'identifiant de l'adresse mail : ");
		fgets(id, sizeof(id), stdin);

		fflush(stdin);
		printf("Veuillez entrer le domaine de l'adresse mail : ");
		fgets(hostname, sizeof(id), stdin);

		id[strlen(id) - 1] = '\0';
		hostname[strlen(hostname) - 1] = '\0';

		e = email_init(id, hostname);
	}
	else
	{
		e = email;

		fflush(stdin);
		printf("Veuillez entrer l'identifiant de l'adresse mail : ");
		fgets(id, sizeof(id), stdin);
		
		id[strlen(id) - 1] = '\0';
		e->id = (char*) malloc((1 + strlen(id)) * sizeof(char));
		strcpy(e->id, id);

		fflush(stdin);
		printf("Veuillez entrer le domaine de l'adresse mail : ");
		fgets(hostname, sizeof(hostname), stdin);
		
		hostname[strlen(hostname) - 1] = '\0';
		e->hostname = (char*) malloc((1 + strlen(hostname)) * sizeof(char));
		strcpy(e->hostname, hostname);
	}

	return e;
}

adress_t* adress_register(adress_t *adress)
{
	adress_t *a = NULL;
	char city[200], pathname[200];
	static unsigned short number = 0;
	static unsigned int postal = 0;
	path_t type;

	printf("\n --- Saisie de l'adresse ---\n");

	if (adress == NULL)
	{
		printf("Veuillez entrer le numero de la rue : ");
		scanf("%d", &number);

		printf("Veuillez entrer le numero correspondant au type de rue\n(0 = Chemin ; 1 = Route ; 2 = Impasse ; 3 = Rue ; 4 = Avenue ; 5 = Boulevard; 6 = Place)\n");
		scanf("%d", &type);

		fflush(stdin);
		printf("Veuillez entrer le nom du chemin : ");
		fgets(pathname, sizeof(pathname), stdin);

		printf("Veuillez entrer le code postal : ");
		scanf("%d", &postal);

		fflush(stdin);
		printf("Veuillez entrer la ville : ");
		fgets(city, sizeof(city), stdin);

		pathname[strlen(pathname) - 1] = '\0';
		city[strlen(city) - 1] = '\0';

		a = adress_init(number, type, pathname, postal, city);
	}
	else
	{
		a = adress;

		printf("Veuillez entrer le numero de la rue : ");
		scanf("%d", &number);
		a->number = number;

		printf("Veuillez entrer le numero correspondant au type de rue\n(0 = Chemin ; 1 = Route ; 2 = Impasse ; 3 = Rue ; 4 = Avenue ; 5 = Boulevard ; 6 = Place)\n");
		scanf("%d", &type);
		a->path = type;

		printf("Veuillez entrer le nom du chemin : ");
		fgets(pathname, sizeof(pathname), stdin);
		fflush(stdin);

		pathname[strlen(pathname) - 1] = '\0';
		a->pathname = (char*) malloc((1 + strlen(pathname)) * sizeof(char));
		strcpy(a->pathname, pathname);
		
		printf("Veuillez entrer le code postal : ");
		scanf("%d", &postal);
		a->postal = postal;

		printf("Veuillez entrer la ville : ");
		fgets(city, sizeof(city), stdin);
		fflush(stdin);

		city[strlen(city) - 1] = '\0';
		a->city = (char*) malloc((1 + strlen(city)) * sizeof(char));
		strcpy(a->city, city);
	}

	return a;
}