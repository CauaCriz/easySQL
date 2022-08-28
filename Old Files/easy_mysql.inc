/*******************************************************

AUTHOR: DeviceBlack
MAIL: dddeviceddd@gmail.com
TIME: 30/07/2022 16:56:09 BRT

Functions:
 - SQL_CreateTable(name[]);
 - SQL_DropTable(name[]);
 - SQL_TableExists(table[]);
 - SQL_AddTableColumn(table[], name[], type[], {Float,_}:length, bool:nullable, bool:primary_key, bool:auto_increment);
 - SQL_DropTableColumn(table[], name[]);
 - SQL_ColumnExists(table[], column[]);

 - SQL_Select(table[], column[], value[]);
 - SQL_ReadInt(column[], &dest, Cache:cache_id);
 - SQL_ReadString(column[], dest[], size, Cache:cache_id);
 - Float:SQL_ReadFloat(column[], &Float:dest, Cache:cache_id);

 - SQL_Insert(table[]);
 - SQL_Update(table[], column[], value[]);
 - SQL_WriteInt(column[], value);
 - SQL_WriteString(column[], value[]);
 - SQL_WriteFloat(column[], Float:value);
 
 - INT(value);
 - FLOAT(value);

*******************************************************/

#include <a_samp>

// ______________ [ HEADER SYSTEM ]
enum COL_VARS
{
	col_type,
	col_name[64],
	int_value,
	str_value[255],
	Float:float_value
}

static
	query[4096], varCol[32][COL_VARS],
	cols, MySQL:server, Cache:sv_cache;


// ______________ [ MAIN FUNCTIONS ]
stock SQL_Connect(const host[], const user[], const pass[], const data[])
{
	server = mysql_connect(host, user, pass, data);
}

stock SQL_CreateTable(const name[])
{
	if(!SQL_TableExists(name))
	{
		format(query, sizeof(query), "CREATE TABLE IF NOT EXISTS %s (dropme int)", name);
		mysql_query(server, query, false);
	}
	else return 0;
	return SQL_TableExists(name);
}

stock SQL_DropTable(const name[])
{
	if(SQL_TableExists(name))
	{
		format(query, sizeof(query), "DROP TABLE IF EXISTS %s", name);
		mysql_query(server, query, false);
	}
	else return 0;
	return !SQL_TableExists(name);
}

stock SQL_TableExists(const name[])
{
	format(query, sizeof(query), "SHOW TABLES LIKE '%s'", name);
	cache_delete(sv_cache);
	sv_cache = mysql_query(server, query);
	return (cache_num_rows() > 0);
}

stock SQL_AddTableColumn(const table[], const name[], const type[], length,
	bool:nullable = true, bool:primary_key = false, bool:auto_increment = false)
{
	if(strcmp(type, "float", true) && strcmp(type, "double", true))
		format(query, sizeof(query), "ALTER TABLE %s ADD %s %s(%d)", table, name, type, length);
	else
		format(query, sizeof(query), "ALTER TABLE %s ADD %s %s", table, name, type);
	
	if(!nullable) format(query, sizeof(query), "%s NOT NULL", query);
	if(primary_key) format(query, sizeof(query), "%s PRIMARY KEY", query);
	if(auto_increment) format(query, sizeof(query), "%s AUTO_INCREMENT", query);
	
	cache_delete(sv_cache);
	sv_cache = mysql_query(server, query);

	SQL_DropTableColumn(table, "dropme");
	return SQL_ColumnExists(table, name);
}

stock SQL_DropTableColumn(const table[], const column[])
{
	format(query, sizeof(query), "ALTER TABLE %s DROP COLUMN %s", table, column);
	cache_delete(sv_cache);
	sv_cache = mysql_query(server, query);
	return !SQL_ColumnExists(table, column);
}

stock SQL_ColumnExists(const table[], const column[])
{
	format(query, sizeof(query), "SHOW COLUMNS FROM `%s` LIKE '%s'", table, column);
	cache_delete(sv_cache);
	sv_cache = mysql_query(server, query);
	return (cache_num_rows() > 0);
}

stock SQL_Select(const table[], const column[], value[])
{
	format(query, sizeof(query), "SELECT * FROM %s WHERE %s = '%s'", table, column, value);
	cache_delete(sv_cache);
	sv_cache = mysql_query(server, query);
	return (cache_num_rows() > 0);
}

stock SQL_Insert(const table[])
{
	format(query, sizeof(query), "INSERT INTO %s (", table);
	for(new i = 0; i < cols; i++)
	{
		if(varCol[i][col_type] == 0) continue;
		else if(i > 0) query[strlen(query)] = 44;
		strcat(query, varCol[i][col_name]);
	}
	
	format(query, sizeof(query), "%s) VALUES (", query);
	for(new i = 0; i < cols; i++)
	{
		if(varCol[i][col_type] == 0) continue;
		else if(i > 0) query[strlen(query)] = 44;

		switch(varCol[i][col_type])
		{
			case 1: format(query, sizeof(query), "%s %d", query, varCol[i][int_value]);
			case 2: format(query, sizeof(query), "%s '%s'", query, varCol[i][str_value]);
			case 3: format(query, sizeof(query), "%s %f", query, varCol[i][float_value]);
		}
	}

	cols = 0;
	query[strlen(query)] = 41;
	
	cache_delete(sv_cache);
	sv_cache = mysql_query(server, query);
}

stock SQL_Update(const table[], const where[], const value[])
{
	format(query, sizeof(query), "UPDATE %s SET", table);
	for(new i = 0; i < cols; i++)
	{
		if(varCol[i][col_type] == 0) continue;
		else if(i > 0) query[strlen(query)] = 44;
		format(query, sizeof(query), "%s %s", query, varCol[i][col_name]);
		
		switch(varCol[i][col_type])
		{
			case 1: format(query, sizeof(query), "%s=%d", query, varCol[i][int_value]);
			case 2: format(query, sizeof(query), "%s='%s'", query, varCol[i][str_value]);
			case 3: format(query, sizeof(query), "%s=%f", query, varCol[i][float_value]);
		}
	}
	
	cols = 0;
	format(query, sizeof(query), "%s WHERE %s='%s'", where, value);
	
	cache_delete(sv_cache);
	sv_cache = mysql_query(server, query);
}


// ______________ [ WRITE FUNCTIONS ]
stock SQL_WriteInt(const column[], value)
{
	varCol[cols][col_type] = 1;
	varCol[cols][int_value] = value;
	format(varCol[cols][col_name], 64, column);
	cols = (cols + 1) % 32;
}

stock SQL_WriteString(const column[], const value[])
{
	varCol[cols][col_type] = 2;
	format(varCol[cols][col_name], 64, column);
	format(varCol[cols][str_value], 255, value);
	cols = (cols + 1) % 32;
}

stock SQL_WriteFloat(const column[], Float:value)
{
	varCol[cols][col_type] = 3;
	varCol[cols][float_value] = value;
	format(varCol[cols][col_name], 64, column);
	cols = (cols + 1) % 32;
}


// ______________ [ READ FUNCTIONS ]
stock SQL_ReadInt(const column[])
{
	static dest = 0;
	if(cache_is_any_active())
	{
		cache_get_value_name_int(0, column, dest);
	}
	return dest;
}

stock SQL_ReadString(const column[], dest[], length = sizeof(dest))
{
	if(cache_is_any_active())
	{
		cache_get_value_name(0, column, dest, length);
	}
	return strlen(dest);
}

stock Float:SQL_ReadFloat(const column[])
{
	static Float:dest = 0.0;
	if(cache_is_any_active())
	{
		cache_get_value_name_float(0, column, dest);
	}
	return dest;
}


// ______________ [ EXTRA FUNCTIONS ]
static convert[255];

stock INT(value)
{
	format(convert, sizeof(convert), "%d", value);
	return convert;
}

stock FLOAT(Float:value)
{
	format(convert, sizeof(convert), "%f", value);
	return convert;
}


