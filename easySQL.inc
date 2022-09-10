#if defined _easysql_
	#endinput
#endif
#define _easysql_

#include <strlib>

static ret, query[255], DB:dados, DBResult:cache;

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
	vInt = db_num_rows(cache); db_free_result(cache);
	return (vInt > 0);
}

stock add_column(const table[], const name[], const type, const length = 11)
{
	if(type == 2) format(vArray, sizeof(vArray), "alter table %s add %s varchar(%d);", table, name, length);
	else if(type == 1) format(vArray, sizeof(vArray), "alter table %s add %s float;", table, name);
	else format(vArray, sizeof(vArray), "alter table %s add %s integer(%d);", table, name, length);
	db_free_result(db_query(dados, vArray));
}

stock column_exists(const table[], const name[])
{
	cache = db_query(dados, sprintf("select * from pragma_table_info('%s') where name = '%s';", table, name));
	vInt = db_num_rows(cache); db_free_result(cache);
	return (vInt > 0);
}


// ---------- read and write table functions ---------- //

static value_str[255], value_int, Float:value_flt;
static DBResult:result;

stock select_table(const table[], const column[], const here_column[], const here_value[])
{
	cache
}

/****** Codigo regeitado
stock read_string(const table[], const column[], const here_column[], const here_value[])
{
	cache = db_query(dados, sprintf("select %s from %s where %s = '%s';", table, name, here_column, here_value));
	db_get_field(cache, 0, vArray, sizeof(vArray)); db_free_result(cache);
	return vArray;
}

stock Float:read_float(const table[], const column[], const here_column[], const here_value[])
{
	cache = db_query(dados, sprintf("select %s from %s where %s = '%s';", table, name, here_column, here_value));
	vFloat = db_get_field_float(cache); db_free_result(cache);
	return vFloat;
}

stock read_int(const table[], const column[], const here_column[], const here_value[])
{
	cache = db_query(dados, sprintf("select %s from %s where %s = '%s';", table, name, here_column, here_value));
	vInt = db_get_field_int(cache); db_free_result(cache);
	return vInt;
}
*******/
