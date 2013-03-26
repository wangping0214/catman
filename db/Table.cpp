#include <catman/db/Table.h>

namespace catman
{
namespace db
{

Table::Table() : m_db(NULL, 0)
{
}

Table::~Table()
{
	m_db.close(0);
}

void Table::open(const std::string &name) throw (DbException)
{
	uint32_t openFlags = DB_CREATE;
	m_db.open(NULL, name.c_str(), NULL, DB_BTREE, openFlags, 0);
}

}
}
