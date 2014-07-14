/*
 * CacheController.h
 *
 *  Created on: Jun 11, 2014
 *      Author: theochen
 */

#ifndef CACHECONTROLLER_H_
#define CACHECONTROLLER_H_


#include "CacheCenter.h"
#include "sdbus/connection.h"

class CacheController {
public:
	CacheController();
	virtual ~CacheController();

	string sqlFetchCache()
	{
		return _sqlFetchCache(_sqlTag);
	}

	void commit();
	void rollback();

	virtual void insertCache(sql::ResultSet* resSet) {}
	virtual void cacheTable(string) {}
	
	template<class T>
	T* getCacheByQuery(dbQuery& q);

	template<class T>
	T* getCacheByQueryInThreadSafty(dbQuery& q);

	template<class T>
	int _getSizeByQuery(dbQuery& q);
	template<class T>
	int _getSizeByQueryInThreadSafty(dbQuery& q);

	template<class T, class T_Ptr>
	std::vector<T_Ptr> getCacheListByQuery(dbQuery& q);

	template<class T, class T_Ptr>
	std::vector<T_Ptr> getCacheListByQueryInThreadSafty(dbQuery& q, bool mutexOn = false);

	template<class T>
	std::map<string, T*> getCacheHashByQuery(dbQuery& q);

public:
	string _class_name;
	string _sqlTag;
	dbDatabase _fstDb;

protected:
	template <class T>
	void _cacheTable(string fetchSQL);

	string _sqlFetchCache(string sqlTag);

	virtual int updateDiskDB(void* cacheAfterUpdate, void* cacheBeforeUpdate) { return 0; }
	virtual int insertDiskDB(void* cache) { return 0; }
	virtual int deleteDiskDB(vector<string>& primaryKeyList) { return 0; }

	template <class T>
	bool _insertCacheInThreadSafty(T* cache, bool mutexOn = true);
	template <class T>
	bool _insertCache(T* cache);

	template <class T>
	void _updateCacheByQueryInThreadSafty(dbQuery& q, 
		void(*callBack)(T*, void* param) = NULL, 
		void* param = NULL, 
		void(*updateSuccessed)(T*, void* param) = NULL,
		void(*updateFailed)(T*, void* param) = NULL,
		bool mutexOn = true);
	template <class T>
	void _updateCacheByQuery(dbQuery& q, 
		void(*callBack)(T*, void* param) = NULL, 
		void* param = NULL,
		void(*updateSuccessed)(T*, void* param) = NULL,
		void(*updateFailed)(T*, void* param) = NULL);

	template <class T>
	bool _deleteCacheByQueryInthreadSafty(dbQuery& q, bool mutexOn = true);

	template <class T>
	bool _deleteCacheByQuery(dbQuery& q);
	
	template <class T>
	void _traverseCacheByQueryInThreadSafty(dbQuery& q, void(*callBack)(T*, void* param) = NULL, void* param = NULL, bool mutexOn = true);
	template<class T>
	void _traverseCacheByQuery(dbQuery& q, void (*callBack)(T*, void* param) = NULL, void* param = NULL);

private:
	void _compiling_error_workaround();

};

#endif /* CACHECONTROLLER_H_ */
