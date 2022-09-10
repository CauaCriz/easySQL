#if defined _easysql_
	#endinput
#endif
#define _easysql_

#include <strlib>

static DB:dados, DBResult:cache;
static ret, query[255];

stock set_database(DB:handle) dados = handle;

stock create_table(const name[])
{
	cache = db_query(dados, sprintf("create table %s (idx int primary key asc);", name));
	db_free_result(cache);
}

stock drop_table(const name[])
{
	cache = db_query(dados, sprintf("drop table %s;", name));
	db_free_result(cache);
}

stock table_exists(const name[])
{
	cache = db_query(dados, sprintf("select * from %s;", name));
	ret = db_num_rows(cache); db_free_result(cache);
	return ret;
}

stock add_column(const table[], const name[], const type, const length = 11)
{
	static query[255];
	if(type == 2) format(query, sizeof(query), "alter table %s add %s varchar(%d);", table, name, length);
	else if(type == 1) format(query, sizeof(query), "alter table %s add %s float;", table, name);
	else format(query, sizeof(query), "alter table %s add %s integer(%d);", table, name, length);
	db_free_result(db_query(dados, query));
}

stock column_exists(const table[], const name[])
{
	cache = db_query(dados, sprintf("select * from pragma_table_info('%s') where name = '%s';", table, name));
	static ret; ret = db_num_rows(cache); db_free_result(cache);
	return ret;
}

stock read_string(const table[], const column[], const here_column[], const here_value[])
{
	cache = db_query(dados, sprintf("select %s from %s where %s = '%s';", table, name, here_column, here_value));
	static dest[255]; db_get_field(cache, 0, dest, sizeof(dest)); db_free_result(cache);
	return dest;
}

stock Float:read_float(const table[], const column[], const here_column[], const here_value[])
{
	cache = db_query(dados, sprintf("select %s from %s where %s = '%s';", table, name, here_column, here_value));
	static Float:value; value = db_get_field_float(cache); db_free_result(cache);
	return value;
}

stock read_int(const table[], const column[], const here_column[], const here_value[])
{
	cache = db_query(dados, sprintf("select %s from %s where %s = '%s';", table, name, here_column, here_value));
	static value; value = db_get_field_int(cache); db_free_result(cache);
	return value;
}

