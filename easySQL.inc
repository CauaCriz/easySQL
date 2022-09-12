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

static r_str DBResult:result;

stock select_table(const table[], const specifier[] = "")
{
	static string[4096];
	format(string, sizeof(string), "select * from %s", table);

	if(specifier[0])
		strcat(string, sprintf(" %s", specifier));

	db_free_result(result);
	result = db_query(dados, string);

	if(db_num_rows(result)) return 1;
	else db_free_result(result);
	return 0;
}

stock read_string(const column[], dest[], const length = sizeof(dest))
{
	if(!db_num_rows(result)) return 0;
	return db_get_field_assoc(result, column, dest, length);
}

stock Float:read_float(const column[])
{
	static dest[32];
	read_string(column, dest);
	return floatstr(dest);
}

stock read_int(const column[])
{
	static dest[32];
	read_string(column, dest);
	return strval(dest);
}


static n_write, n_str[4096];

stock write_string(const column[], const value[])
{
	if(!(0 < strlen(column) < 17)) return 0;
	else if(!(0 < strlen(value) < 32)) return 0;

	SetSVarString(sprintf("sql_column:%d", n_write), column);
	SetSVarString(sprintf("sql_value:%d", n_write++), column);
	return 1;
}

stock write_float(const column[], const Float:value)
{
	return write_string(column, sprintf("%f", value));
}

stock write_int(const column[], const value)
{
	return write_string(column, sprintf("%d", value));
}

stock insert_table(const table[])
{
	static string[4096];
	for(
}




