#ifndef TABLE_H
#define TABLE_H
/**************************************************************
 * (C) Copyright 2013 Alanmars
 * Keep it simple at first 
 *************************************************************/

#include <catman/common/OctetsStream.h>
#include <db_cxx.h>
#include <string>

namespace catman
{
namespace db
{

class Database;

class Table
{
	friend class Database;
public:
	template<typename K, typename V> class Traverser
	{
	public:
		virtual void traverse(K &key, V &value) = 0;
	};
public:
	virtual ~Table();
	template<typename K, typename V>  int32_t get(const K &key, V &value) throw (DbException)
	{
		common::OctetsStream os;
		os << key;
		Dbt kdbt(os.begin(), os.size());
		Dbt vdbt;
		m_db->get(NULL, &kdbt, &vdbt, 0);
		common::OctetsStream vos(common::Octets(vdbt.get_data(), vdbt.get_size()));
		vos >> value;
		return 0;
	}
	template<typename K, typename V> int32_t put(const K &key, const V &value) throw (DbException)
	{
		common::OctetsStream kos;
		kos << key;
		common::OctetsStream vos;
		vos << value;
		Dbt kdbt(kos.begin(), kos.size()), vdbt(vos.begin(), vos.size());
		return m_db->put(NULL, &kdbt, &vdbt, 0);
	}
	template<typename K> int32_t del(const K &key) throw (DbException)
	{
		common::OctetsStream kos;
		kos << key;
		Dbt kdbt(kos.begin(), kos.size());
		return m_db->del(NULL, &kdbt, 0);
	}
	template<typename K, typename V> void traverse(Traverser<K, V> &traverser) throw (DbException)
	{
		Dbc *cursorPtr;
		m_db->cursor(NULL, &cursorPtr, 0);
		Dbt key, value;
		while (cursorPtr->get(&key, &value, DB_NEXT) == 0)
		{
			K k;
			V v;
			common::OctetsStream kos(common::Octets(key.get_data(), key.get_size()));
			kos >> k;
			common::OctetsStream vos(common::Octets(value.get_data(), value.get_size()));
			vos >> v;
			traverser.traverse(k, v);
		}
		cursorPtr->close();
	}
private:
	Table(Db *db);
private:
	Db *m_db;
};

}
}

#endif

