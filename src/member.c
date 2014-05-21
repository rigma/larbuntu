#include <stdlib.h>
#include <stdio.h>
#include <member.h>

member_t *member_init()
{
	member_t *member = (member_t*) malloc(sizeof(member_t));
	if (member == NULL)
		return NULL;

	member->name = NULL;
	member->forname = NULL;
	member->adress = NULL;
	member->email = NULL;
	member->profession = NULL;
	member->n_borrows = 0;
	member->borrows = NULL;
	member->previous = NULL;
	member->next = NULL;
	
	return member;
}

void member_free(member_t *member)
{
	free(member->name);
	free(member->forname);
	free(member->profession);
	free(member->borrows);
	free(member);
}

member_t* member_register(member_t* member)
{
	member_t *m = NULL;
	char tempo[200];

	if (member == NULL)
		m = member_init();
	else
		m = member;

	printf("Veuillez entrer le nom de l adherent");
	fgets(tempo, sizeof(tempo), stdin);
	fflush(stdin);

	m->name = (char*) malloc((1 + strlen(tempo)) * sizeof(char));

	printf("Veuillez entrer le prenom de l adhherent");
	fgets(tempo, sizeof(tempo), stdin);
	fflush(stdin);

	m->forname = (char*) malloc((1 + strlen(tempo)) * sizeof(char));

	// saisie postale
	m->email = email_register(NULL);

	printf("Veuillez entrer la profession de l adherent");
	fgets(tempo, sizeof(tempo), stdin);
	fflush(stdin);

	m->profession = (char*)malloc((1 + strlen(tempo)) * sizeof(char)); 

	return m;
}