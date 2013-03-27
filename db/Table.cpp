#include <catman/db/Table.h>

namespace catman
{
namespace db
{

Table::Table(Db *db) : m_db(db)
{
}

Table::~Table()
{
	m_db->close(0);
}

}
}
