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

static DBResult:result;

stock select_table(const table[], const here_column[], const here_value[])
{
	result = db_open(dados, sprintf("select * from %s where %s = '%s';", table, here_column, here_value));
	if(db_num_rows(result)) return 1;
	else db_free_result(result);
	return 0;
}

stock Float:read_float(const column[])
{
	if(db_num_rows(result))
	{
		return db_get_field_assoc_float(result, column);
	}
	return 0.0;
}

stock read_int(const column[])
{
	if(db_num_rows(result))
	{
		return db_get_field_assoc_int(result, column);
	}
	return 0;
}

stock read_string(const column[], dest[], const length = sizeof(dest))
{
	if(db_num_rows(result))
	{
		return db_get_field_assoc(result, column, dest, length);
	}
	return 0;
}

static write_column[4096], write_value[4096];

stock write_float(const column[], const Float:value)
{
	format(write_column, sizeof(write_column), "%s
}


