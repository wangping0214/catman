#include <catman/db/Table.h>
#include <catman/db/Database.h>
#include <RegisterArg>
#include <iostream>

template<typename K, typename V> class SimpleTraverser : public catman::db::Table::Traverser<K, V>
{
public:
	void traverse(K &key, V &value)
	{
	}
};

class SecTraverser : public catman::db::Table::Traverser<int, protocol::RegisterArg>
{
public:
	void traverse(int &key, protocol::RegisterArg &value)
	{
	}
};

int main(int argc, char *argv[])
{
	/*
	catman::db::Table table;
	table.open("mydb.db");
	*/
	catman::db::Database db;
	db.open("dbhome");
	catman::db::Table table = db.getTable("player");
	protocol::RegisterArg arg;
	/*
	arg.userName = "wangping";
	arg.password = "admin";
	table.put(1, arg);
	*/
	table.get(1, arg);
	std::cout << arg.userName << std::endl;
	std::cout << arg.password << std::endl;
//	SimpleTraverser<int, RegisterArg> st;
	SecTraverser st;
	table.traverse(st);

	return 0;
}
