#include <stdlib.h>

#include <db.h>

db_t *db_init(char *member, char *thema, char *book)
{
	db_t *db = (db_t*) malloc(sizeof(db_t));
	if (db == NULL)
		return NULL;

	db->db_member = member_initDatabase(member);
	if (db->db_member == NULL)
	{
		free(db);

		return NULL;
	}

	db->db_book = book_initDatabase(book);
	if (db->db_book == NULL)
	{
		member_freeDatabase(db->db_member);
		free(db);

		return NULL;
	}

	db->db_thema = thema_initDatabase(db->db_book, thema);
	if (db->db_thema == NULL)
	{
		member_freeDatabase(db->db_member);
		book_freeDatabase(db->db_member);
		free(db);

		return NULL;
	}

	return db;
}

char db_save(db_t *db)
{
	return member_saveDatabase(db->db_member) && thema_saveDatabase(db->db_thema) && book_saveDatabase(db->db_book);
}

char db_free(db_t *db)
{
	if (member_freeDatabase(db->db_member) && thema_freeDatabase(db->db_thema) && book_freeDatabase(db->db_book))
	{
		free(db);

		return 1;
	}
	else
		return 0;
}
