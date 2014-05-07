#include <stdlib.h>
#include <member.h>

member_t *member_init()
{
	member_t *member = (member_t*) malloc(sizeof(member_t));
	if (member == NULL)
		return NULL;

	member->name = NULL;
	member->forname = NULL;
	member->profession = NULL;
	member->n_borrows = 0;
	member->borrows = NULL;
	member->previous = NULL;
	member->next = NULL;
	
	return member;
}

void member_free(member_t* member)
{
	free(member->name);
	free(member->forname);
	free(member->profession);
	free(member->borrows);
	free(member);
}
