#include	<a_samp>
#include	<easysql>

public OnGameModeInit()
{
	open_database("test.db");
	if(table_exists("EasySQL"))
	{
		if(select_table("EasySQL", "limit 1"))
		{
			new
				idx = read_int("idx"),
				ret = read_int("Test");

			printf("Valor Test: %d", ret);
			
			write_int("Test", random(100));
			update_table("Test", sprintf("where idx = '%d'", idx));
		}
		else printf("Falha ao carregar dados");
	}
	else
	{
		create_table("EasySQL");
		add_column("EasySQL", "Test", 2);

		write_int("Test", random(100));
		insert_table("EasySQL");
	}
	return 1;
}

main(){}

