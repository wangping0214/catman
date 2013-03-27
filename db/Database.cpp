#include <catman/db/Database.h>
#include <catman/db/Table.h>
#include <catman/common/Toolkit.h>

namespace catman
{
namespace db
{

Database::Database() : m_env(0)
{
}

Database::~Database()
{
	m_env.close(0);
}

void Database::open(const std::string &dbDirPath) throw (DbException)
{
	if (!catman::common::MakePath(dbDirPath.c_str(), 0776))
		return;
	uint32_t envFlags = DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN;
	m_env.open(dbDirPath.c_str(), envFlags, 0);
}

Table Database::getTable(const std::string &tableName) throw (DbException)
{
	uint32_t dbFlags = DB_CREATE | DB_AUTO_COMMIT;
	Db *db = new Db(&m_env, 0);
	db->open(NULL, tableName.c_str(), NULL, DB_BTREE, dbFlags, 0);
	return Table(db);
}

}
}

