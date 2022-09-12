#if defined fake_native
function set_database(DB:handle); // setar banco de dados
function create_table(const name[]); // criar tabela
function drop_table(const name[]); // deletar tabela
function table_exists(const name[]); // verificar se tabela existe
function add_column(const table[], const name[], const type, const length = 11); // adicionar coluna na tabela
function column_exists(const table[], const name[]); // verificar se coluna existe na tabela
function select_table(const table[], const specifier[] = ""); // selecionar resultados na tabela
function insert_table(const table[]); // inserir resultados na tabela
function update_table(const table[], const specifier[] = ""); // atualizar resultados na tabela

function read_string(const column[], dest[]); // recuperar string em uma tabela especifica
function Float:read_float(const column[]); // recuperar float em uma tabela especifica
function read_int(const column[]); // recuperar int em uma tabela especifica

function write_string(const column[], const value[]); // pre-escrever o valor da string na coluna
function write_float(const column[], const Float:value); // pre-escrever o valor flutuante na coluna
function write_int(const column[], const value); // pre-escrever o valor inteiro na coluna
#endif

#if defined _easysql_
	#endinput
#endif
#define _easysql_

#include <strlib>

static ret, query[255], DB:dados, DBResult:cache;

stock set_database(const DB:handle) dados = handle;

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

static r_str[4096], DBResult:result;

stock select_table(const table[], const specifier[] = "")
{
	format(r_str, sizeof(r_str), "select * from %s", table);

	if(specifier[0])
		strcat(r_str, sprintf(" %s", specifier));

	db_free_result(result);
	result = db_query(dados, r_str);

	if(db_num_rows(result)) return 1;
	else db_free_result(result);
	return 0;
}

stock read_string(const column[], dest[])
{
	if(!db_num_rows(result)) return 0;
	return db_get_field_assoc(result, column, dest, 32);
}

stock Float:read_float(const column[])
{
	read_string(column, r_str);
	return floatstr(r_str);
}

stock read_int(const column[])
{
	read_string(column, r_str);
	return strval(r_str);
}


static n_write, w_i, w_str[4096];

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
	format(w_str, sizeof(w_str), "insert into %s (", table);
	for(w_i = 0; w_i < n_write; w_i++)
	{
		static column[16];
		GetSVarString(sprintf("sql_column:%d", w_i), column, sizeof(column));
		DeleteSVar(sprintf("sql_column:%d", w_i));

		format(w_str, sizeof(w_str), "%s%s", w_str, column);
		if((w_i + 1) < n_write) strcat(w_str, ",");
	}

	strcat(w_str, ") values (");
	for(w_i = 0; w_i < n_write; w_i++)
	{
		static value[32];
		GetSVarString(sprintf("sql_value:%d", w_i), value, sizeof(value));
		DeleteSVar(sprintf("sql_value:%d", w_i));

		format(w_str, sizeof(w_str), "%s'%s'", w_str, value);
		if((w_i + 1) < n_write) strcat(w_str, ",");
	}

	strcat(w_str, ")");
	db_free_result(db_query(dados, w_str));
	return select_table(table);
}

stock update_table(const table[], const specifier[] = "")
{
	format(w_str, sizeof(w_str), "update %s set", table);
	for(w_i = 0; w_i < n_write; w_i++)
	{
		static column[16], value[32];
		GetSVarString(sprintf("sql_column:%d", w_i), column, sizeof(column));
		GetSVarString(sprintf("sql_value:%d", w_i), value, sizeof(value));
		DeleteSVar(sprintf("sql_column:%d", w_i));
		DeleteSVar(sprintf("sql_value:%d", w_i));

		format(w_str, sizeof(w_str), "%s %s='%s'", w_str, column, value);
		if((w_i + 1) < n_write) strcat(w_str, ",");
	}
	
	if(specifier[0])
		strcat(w_str, sprintf(" %s", specifier));

	db_free_result(db_query(dados, w_str));
	return select_table(table, specifier);
}



