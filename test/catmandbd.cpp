#include <catman/db/Table.h>
#include <catman/db/Database.h>
#include <RegisterArg>
#include <iostream>

int main(int argc, char *argv[])
{
	/*
	catman::db::Table table;
	table.open("mydb.db");
	*/
	catman::db::Database db;
	db.open("dbhome");
	catman::db::Table table = db.getTable("player");
	RegisterArg arg;
	/*
	arg.userName = "wangping";
	arg.password = "admin";
	table.put(1, arg);
	*/
	table.get(1, arg);
	std::cout << arg.userName << std::endl;
	std::cout << arg.password << std::endl;

	return 0;
}
