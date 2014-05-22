#include <stdlib.h>
#include <stdio.h>

#include <utils.h>

char *datetime(const struct tm *timeptr)
{
	static const char wday[][9] = { "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi" };
	static const char mon[][10] = {
		"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"
	};
	char *result = NULL;

	result = (char*) malloc(42 * sizeof(char));
	if (result == NULL)
		return NULL;

	// JJ DD MM AAAA | HH:MM:SS
	sprintf(result, "%s %d %s %d | %.2d:%.2d:%.2d\n",
		wday[timeptr->tm_wday],
		timeptr->tm_mday,
		mon[timeptr->tm_mon],
		1900 + timeptr->tm_year,
		timeptr->tm_hour,
		timeptr->tm_min,
		timeptr->tm_sec);

	return result;
}
