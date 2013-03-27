#ifndef DATABASE_H
#define DATABASE_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/db/Table.h>
#include <db_cxx.h>

namespace catman
{
namespace db
{

class Database
{
public:
	Database();
	~Database();
	void open(const std::string &dbDirPath) throw (DbException);
	Table getTable(const std::string &tableName) throw (DbException);
private:
	DbEnv m_env;
};

}
}

#endif

