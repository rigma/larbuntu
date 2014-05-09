#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dbio.h>
#include <thema.h>

thema_t *thema_init(char *key)
{
	thema_t *thema = (thema_t*) malloc(sizeof(thema_t));
	if (thema == NULL)
		return NULL;

	strcpy(thema->key, key);

	thema->id = 0;
	thema->size = 0;
	thema->books = NULL;
	thema->previous = NULL;
	thema->next = NULL;

	return thema;
}

void thema_free(thema_t *thema)
{
	free(thema->books);
	free(thema);
}
