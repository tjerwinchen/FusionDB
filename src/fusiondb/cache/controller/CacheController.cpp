/*
 * CacheController.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: theochen
 */

#include "../stdafx.h"
#include "CacheController.h"

CacheController::CacheController() {
    // TODO Auto-generated constructor stub
    _class_name = "CacheController";
    _sqlTag = "";
}

CacheController::~CacheController() {
    // TODO Auto-generated destructor stub
}

string CacheController::_sqlFetchCache(string sqlTag) {
    pugi::xml_document doc;
#ifdef WINDOWS_PLATFORM
    pugi::xml_parse_result result =
            doc.load_file("..\\Resource\\sqlquery.xml");
#else
	pugi::xml_parse_result result =
		doc.load_file("../Resource/sqlquery.xml");
#endif
	
    if (result.status != pugi::status_ok) {
        cout << "Load result: " << result.description() << endl;
        return "";
    }
    string fetchSQL = doc.child("SQL").child(sqlTag.c_str()).child_value();

    return fetchSQL;
}

void CacheController::commit()
{
	CacheCenter::sharedInstance()->CommitCache();
}

void CacheController::rollback()
{
	CacheCenter::sharedInstance()->RollbackCache();
}

template <class T>
void CacheController::_cacheTable(string fetchSQL)
{
    if (fetchSQL.length() == 0) {
        return;
    }
	std::string oldSchema = DBConnector::getConnector()->getSchema(); 
	DBConnector::getConnector()->setSchema(T::SCHEMA_NAME);
    sql::ResultSet* resSet = DBConnector::getConnector()->executeQuery(
            fetchSQL);
    cout << "cache... " << _class_name << " - Result Set: " << resSet->rowsCount() << endl;
    try {
		CacheCenter::sharedInstance()->AttachCurrentThread();

        dbCursor<T> cursor(dbCursorForUpdate);
        cursor.removeAll();

        while (resSet->next()) {	
            insertCache(resSet);
        }
        delete resSet;
        cout << "Successfully Cached ... " << _class_name << endl;

		CacheCenter::sharedInstance()->DetachCurrentThread();
    } catch (dbException e) {
        cout << e.getMsg() << endl;
    }

	DBConnector::getConnector()->setSchema(oldSchema);
}

template <class T>
bool CacheController::_insertCache(T* cache) {
	bool ret = false;
	try {	
		CacheCenter::sharedInstance()->_fstDb.insert(*cache);
		int effectedLines = insertDiskDB(cache);
		assert(effectedLines <= 1);
		if (effectedLines == 1)
		{
			ret = true;
			commit();
		}
		else
		{
			rollback();
		}
		
	} catch (dbException e) {
		cout << e.getMsg() << endl;
	}

	return ret;
} 	

template <class T>
bool CacheController::_insertCacheInThreadSafty(T* cache, bool mutexOn) {
	
	CacheCenter::sharedInstance()->AttachCurrentThread();

	bool ret = false;

	dbMutex mutex;
	if (mutexOn)
	{
		mutex.lock();
		ret = _insertCache(cache);
		mutex.unlock();
	}
	else
	{
		ret = _insertCache(cache);
	}

	CacheCenter::sharedInstance()->DetachCurrentThread();

	return ret;
} 	

template <class T>
void CacheController::_updateCacheByQueryInThreadSafty(dbQuery& q, 
	void(*callBack)(T*, void* param),
	void* param, 
	void(*updateSuccessed)(T*, void* param),
	void(*updateFailed)(T*, void* param),
	bool mutexOn) {

	CacheCenter::sharedInstance()->AttachCurrentThread();
	dbMutex mutex;
	if (mutexOn)
	{
		mutex.lock();
		_updateCacheByQuery<T>(q, callBack, param, updateSuccessed, updateFailed);
		mutex.unlock();
	}
	else
	{
		_updateCacheByQuery<T>(q, callBack, param, updateSuccessed, updateFailed);
	}

	CacheCenter::sharedInstance()->DetachCurrentThread();
}

template <class T>
void CacheController::_updateCacheByQuery(dbQuery& q, 
	void(*callBack)(T*, void* param), 
	void* param, 
	void(*updateSuccessed)(T*, void* param),
	void(*updateFailed)(T*, void* param)
	) {
	TIME_COST_BEGIN("Update Cache...");
	T* tempCache = NULL;
	try {	
		dbCursor<T> cursor(dbCursorForUpdate);

		int resultLine = cursor.select(q);
		cout << "Need to update " << resultLine << " lines in Cache - " << T::TABLE_NAME<< endl;

		do {
			T* cache = cursor.get();
			tempCache = cache;
			if (cache && callBack) {
				T oldCache = *cache;
				callBack(cache, param);	 
				
				cursor.update();	 
				int effectedLines = updateDiskDB(cache, &oldCache);
				assert(effectedLines <= 1);
				if (effectedLines == 1)
				{
					commit();
					if (updateSuccessed)
					{
						updateSuccessed(cache, param);
					}
				}
				else
				{
					rollback();
					if (updateFailed)
					{
						updateFailed(cache, param);
					}
				}
			}
		} while (cursor.next());
	} catch (dbException e) {
		cout << e.getMsg() << endl;
		updateFailed(tempCache, param);
	}
	TIME_COST_END("Update Cache...");
}

template <class T>
bool CacheController::_deleteCacheByQueryInthreadSafty(dbQuery& q, bool mutexOn)
{
	bool retResult = false;
	CacheCenter::sharedInstance()->AttachCurrentThread();
	dbMutex mutex;
	if (mutexOn)
	{
		mutex.lock();
		retResult = _deleteCacheByQuery<T>(q);
		mutex.unlock();
	}
	else
	{
		retResult = _deleteCacheByQuery<T>(q);
	}

	CacheCenter::sharedInstance()->DetachCurrentThread();
	return retResult;
}


template <class T>
bool CacheController::_deleteCacheByQuery(dbQuery& q)
{
	bool retResult = false;
	TIME_COST_BEGIN("Delete Cache...");
	T* tempCache = NULL;
	try {	
		dbCursor<T> cursor(dbCursorForUpdate);

		int resultLine = cursor.select(q);
		cout << "Need to delete " << resultLine << " lines in Cache - " << T::TABLE_NAME	<< endl;

		vector<string> primaryKeyList;
		for (int i=0; i<resultLine; i++)
		{
			T* cache = cursor.get();
			
			if (cache)
			{
				primaryKeyList.push_back("'"+cache->getPrimaryKey()+"'");
				cursor.remove();
			}
		}
		if (primaryKeyList.size() > 0)
		{
			int effectedLines = deleteDiskDB(primaryKeyList);
			if (effectedLines > 0)
			{
				assert(effectedLines == primaryKeyList.size());
				commit();
				retResult = true;
			}
			cout << "Delete " << primaryKeyList.size() << " rows in FastDB <----> Delete " << effectedLines << " rows in MySQL" << endl;
		}
	}
	catch (dbException e) {
		cout << e.getMsg() << endl;
	}

	TIME_COST_END("Delete Cache...");
	return retResult;
}

/*
template <class T>
void CacheController::_deleteCacheByQuery(dbQuery& q,
	void* param, 
	void(*deleteSuccessed)(T*, void* param),
	void(*deleteFailed)(T*, void* param)
	)
{
	TIME_COST_BEGIN("Delete Cache...");
	T* tempCache = NULL;
	try {	
		dbCursor<T> cursor(dbCursorForUpdate);

		int resultLine = cursor.select(q);
		cout << "Need to delete " << resultLine << " lines in Cache - " << T::TABLE_NAME	<< endl;

		for (int i=0; i<resultLine; i++)
		{
			T* cache = cursor.get();
			tempCache = cache;
			if (cache) { 
				int effectedLines = deleteDiskDB(cache);
				assert(effectedLines <= 1);
				if (effectedLines == 1)
				{
					cursor.remove();
					if (deleteSuccessed)
					{
						deleteSuccessed(cache, param);
					}
				}
				else
				{
					cursor.next();
					if (deleteFailed)
					{
						deleteFailed(cache, param);
					}
				}	
			}
		}
		commit();
	}
	catch (dbException e) {
		cout << e.getMsg() << endl;
		deleteFailed(tempCache, param);
	}
	TIME_COST_END("Delete Cache...");
}
*/

template <class T>
void CacheController::_traverseCacheByQueryInThreadSafty(dbQuery& q, void(*callBack)(T*, void* param), void* param, bool mutexOn) {

	CacheCenter::sharedInstance()->AttachCurrentThread();
	dbMutex mutex;
	if (mutexOn)
	{
		mutex.lock();
		_traverseCacheByQuery<T>(q, callBack, param);
		mutex.unlock();
	}
	else
	{
		 _traverseCacheByQuery<T>(q, callBack, param);
	}
	
	CacheCenter::sharedInstance()->DetachCurrentThread();
}

template <class T>
void CacheController::_traverseCacheByQuery(dbQuery& q, void(*callBack)(T*, void* param), void* param) {
    try {	
        dbCursor<T> cursor(dbCursorViewOnly);

     	TIME_COST_BEGIN("Get Cache...");
        int resultLine = cursor.select(q);
        cout << "Get Cache... - Results Lines: " << resultLine	<< endl;
		TIME_COST_END("Get Cache...");

        do {
            T* cache = cursor.get();
            if (cache && callBack) {
                callBack(cache, param);
            }
        } while (cursor.next());
    } catch (dbException e) {
        cout << e.getMsg() << endl;
    }
}

template<class T>
T* CacheController::getCacheByQuery(dbQuery& q)
{
	dbCursor<T> cursor(dbCursorViewOnly);

	T* cache = NULL;
	int resultSize = cursor.select(q);
	assert(resultSize <= 1);

	if (resultSize > 0)
	{
		cache = new T();
		*cache = *cursor.get();
	}

	return cache;
}

template<class T>
int CacheController::_getSizeByQuery(dbQuery& q)
{
	dbCursor<T> cursor(dbCursorViewOnly);

	return cursor.select(q);
}

template<class T>
int CacheController::_getSizeByQueryInThreadSafty(dbQuery& q)
{
	CacheCenter::sharedInstance()->AttachCurrentThread();
	int size = _getSizeByQuery<T>(q);
	CacheCenter::sharedInstance()->DetachCurrentThread();
	return size;
}

template<class T>
T* CacheController::getCacheByQueryInThreadSafty(dbQuery& q)
{
	CacheCenter::sharedInstance()->AttachCurrentThread();
	T* cache = getCacheByQuery<T>(q);
	CacheCenter::sharedInstance()->DetachCurrentThread();
	return cache;
}

template<class T>
std::map<string, T*> CacheController::getCacheHashByQuery(dbQuery& q)
{
	std::map<string, T*> cacheList;
	dbCursor<T> cursor(dbCursorViewOnly);

	T* cache = NULL;
	int resultSize = cursor.select(q);
	if (resultSize > 0)
	{
		do
		{
			cache = new T();
			*cache = *cursor.get();
			cacheList.insert(std::pair<string, T*>(cache->getPrimaryKey(), cache));
		}
		while(cursor.next());
	}
	
	return cacheList;
}

template<class T, class T_Ptr>
std::vector<T_Ptr> CacheController::getCacheListByQuery(dbQuery& q) 
{
	std::vector<T_Ptr> cachePtrList;
	dbCursor<T> cursor(dbCursorViewOnly);

	int resultSize = cursor.select(q);
	if (resultSize > 0)
	{
		do
		{
			T* cache = new T();
			*cache = *cursor.get();
			T_Ptr cachePtr(cache);
			cachePtrList.push_back(cachePtr);
		}
		while(cursor.next());
	}

	return cachePtrList;
}

template<class T, class T_Ptr>
std::vector<T_Ptr> CacheController::getCacheListByQueryInThreadSafty(dbQuery& q, bool mutexOn) 
{
	std::vector<T_Ptr> cacheList;
	CacheCenter::sharedInstance()->AttachCurrentThread();
	if (mutexOn)
	{
		dbMutex mutex;
		mutex.lock();
		cacheList = getCacheListByQuery<T, T_Ptr>(q);
		mutex.unlock();
	}
	else
	{
		cacheList = getCacheListByQuery<T, T_Ptr>(q);
	}
	CacheCenter::sharedInstance()->DetachCurrentThread();
	return cacheList;
}

/*
 * @Description: I am not sure the root cause of this weird scenario: If you want the template functions
 *         be called from derived class. You should to call it in the parent class. Or the compiler will not
 *         let it go.
 */
void CacheController::_compiling_error_workaround()
{
    _cacheTable<SelectiveInfoCache>("");
    _cacheTable<BondSmallViewCache>("");
    _cacheTable<BondQuoteCache>("");
    _cacheTable<BondQuoteReferCache>("");
    _cacheTable<BondBestQuoteCache>("");
    _cacheTable<BondDealCache>("");
    _cacheTable<BondTraderCache>("");
    _cacheTable<ChinaBondValuationCache>("");
	_cacheTable<IssuingInstitutionCache>("");
	_cacheTable<TradingInstitutionCache>("");
    _cacheTable<BondGroupCache>("");
    _cacheTable<BondGroupDetailCache>("");
	_cacheTable<AccountCache>("");
	_cacheTable<BondTypeSelectiveCache>("");

    dbQuery q;
    _traverseCacheByQuery<SelectiveInfoCache>(q);
    _traverseCacheByQuery<BondSmallViewCache>(q);
    _traverseCacheByQuery<BondQuoteCache>(q);
    _traverseCacheByQuery<BondQuoteReferCache>(q);
    _traverseCacheByQuery<BondBestQuoteCache>(q);
    _traverseCacheByQuery<BondDealCache>(q);
    _traverseCacheByQuery<BondTraderCache>(q);
    _traverseCacheByQuery<ChinaBondValuationCache>(q);
	_traverseCacheByQuery<IssuingInstitutionCache>(q);
	_traverseCacheByQuery<TradingInstitutionCache>(q);
    _traverseCacheByQuery<BondGroupCache>(q);
    _traverseCacheByQuery<BondGroupDetailCache>(q);
	_traverseCacheByQuery<AccountCache>(q);
	_traverseCacheByQuery<BondTypeSelectiveCache>(q);

	_traverseCacheByQueryInThreadSafty<SelectiveInfoCache>(q);
	_traverseCacheByQueryInThreadSafty<BondSmallViewCache>(q);
	_traverseCacheByQueryInThreadSafty<BondQuoteCache>(q);
	_traverseCacheByQueryInThreadSafty<BondQuoteReferCache>(q);
	_traverseCacheByQueryInThreadSafty<BondBestQuoteCache>(q);
	_traverseCacheByQueryInThreadSafty<BondDealCache>(q);
	_traverseCacheByQueryInThreadSafty<BondTraderCache>(q);
	_traverseCacheByQueryInThreadSafty<ChinaBondValuationCache>(q);
	_traverseCacheByQueryInThreadSafty<IssuingInstitutionCache>(q);
	_traverseCacheByQueryInThreadSafty<TradingInstitutionCache>(q);
    _traverseCacheByQueryInThreadSafty<BondGroupCache>(q);
    _traverseCacheByQueryInThreadSafty<BondGroupDetailCache>(q);
	_traverseCacheByQueryInThreadSafty<AccountCache>(q);
	_traverseCacheByQueryInThreadSafty<BondTypeSelectiveCache>(q);

	getCacheByQuery<SelectiveInfoCache>(q);
	getCacheByQuery<BondSmallViewCache>(q);
	getCacheByQuery<BondQuoteCache>(q);
	getCacheByQuery<BondQuoteReferCache>(q);
	getCacheByQuery<BondBestQuoteCache>(q);
	getCacheByQuery<BondDealCache>(q);
	getCacheByQuery<BondTraderCache>(q);
	getCacheByQuery<ChinaBondValuationCache>(q);
	getCacheByQuery<IssuingInstitutionCache>(q);
	getCacheByQuery<TradingInstitutionCache>(q);
    getCacheByQuery<BondGroupCache>(q);
    getCacheByQuery<BondGroupDetailCache>(q);
	getCacheByQuery<AccountCache>(q);
	getCacheByQuery<BondTypeSelectiveCache>(q);

	getCacheByQueryInThreadSafty<SelectiveInfoCache>(q);
	getCacheByQueryInThreadSafty<BondSmallViewCache>(q);
	getCacheByQueryInThreadSafty<BondQuoteCache>(q);
	getCacheByQueryInThreadSafty<BondQuoteReferCache>(q);
	getCacheByQueryInThreadSafty<BondBestQuoteCache>(q);
	getCacheByQueryInThreadSafty<BondDealCache>(q);
	getCacheByQueryInThreadSafty<BondTraderCache>(q);
	getCacheByQueryInThreadSafty<ChinaBondValuationCache>(q);
	getCacheByQueryInThreadSafty<IssuingInstitutionCache>(q);
	getCacheByQueryInThreadSafty<TradingInstitutionCache>(q);
    getCacheByQueryInThreadSafty<BondGroupCache>(q);
    getCacheByQueryInThreadSafty<BondGroupDetailCache>(q);
	getCacheByQueryInThreadSafty<AccountCache>(q);
	getCacheByQueryInThreadSafty<BondTypeSelectiveCache>(q);

	_getSizeByQuery<SelectiveInfoCache>(q);
	_getSizeByQuery<BondSmallViewCache>(q);
	_getSizeByQuery<BondQuoteCache>(q);
	_getSizeByQuery<BondQuoteReferCache>(q);
	_getSizeByQuery<BondBestQuoteCache>(q);
	_getSizeByQuery<BondDealCache>(q);
	_getSizeByQuery<BondTraderCache>(q);
	_getSizeByQuery<ChinaBondValuationCache>(q);
	_getSizeByQuery<IssuingInstitutionCache>(q);
	_getSizeByQuery<TradingInstitutionCache>(q);
	_getSizeByQuery<BondGroupCache>(q);
	_getSizeByQuery<BondGroupDetailCache>(q);
	_getSizeByQuery<AccountCache>(q);
	_getSizeByQuery<BondTypeSelectiveCache>(q);

	_getSizeByQueryInThreadSafty<SelectiveInfoCache>(q);
	_getSizeByQueryInThreadSafty<BondSmallViewCache>(q);
	_getSizeByQueryInThreadSafty<BondQuoteCache>(q);
	_getSizeByQueryInThreadSafty<BondQuoteReferCache>(q);
	_getSizeByQueryInThreadSafty<BondBestQuoteCache>(q);
	_getSizeByQueryInThreadSafty<BondDealCache>(q);
	_getSizeByQueryInThreadSafty<BondTraderCache>(q);
	_getSizeByQueryInThreadSafty<ChinaBondValuationCache>(q);
	_getSizeByQueryInThreadSafty<IssuingInstitutionCache>(q);
	_getSizeByQueryInThreadSafty<TradingInstitutionCache>(q);
	_getSizeByQueryInThreadSafty<BondGroupCache>(q);
	_getSizeByQueryInThreadSafty<BondGroupDetailCache>(q);
	_getSizeByQueryInThreadSafty<AccountCache>(q);
	_getSizeByQueryInThreadSafty<BondTypeSelectiveCache>(q);

	getCacheListByQueryInThreadSafty<SelectiveInfoCache, SelectiveInfoCachePtr>(q);
	getCacheListByQueryInThreadSafty<BondSmallViewCache, BondSmallViewCachePtr>(q);
	getCacheListByQueryInThreadSafty<BondQuoteCache, BondQuoteCachePtr>(q);
	getCacheListByQueryInThreadSafty<BondQuoteReferCache, BondQuoteReferCachePtr>(q);
	getCacheListByQueryInThreadSafty<BondBestQuoteCache, BondBestQuoteCachePtr>(q);
	getCacheListByQueryInThreadSafty<BondDealCache, BondDealCachePtr>(q);
	getCacheListByQueryInThreadSafty<BondTraderCache, BondTraderCachePtr>(q);
	getCacheListByQueryInThreadSafty<ChinaBondValuationCache, ChinaBondValuationCachePtr>(q);
	getCacheListByQueryInThreadSafty<IssuingInstitutionCache, IssuingInstitutionCachePtr>(q);
	getCacheListByQueryInThreadSafty<TradingInstitutionCache, TradingInstitutionCachePtr>(q);
    getCacheListByQueryInThreadSafty<BondGroupCache, BondGroupCachePtr>(q);
    getCacheListByQueryInThreadSafty<BondGroupDetailCache, BondGroupDetailCachePtr>(q);
	getCacheListByQueryInThreadSafty<AccountCache, AccountCachePtr>(q);
	getCacheListByQueryInThreadSafty<BondTypeSelectiveCache, BondTypeSelectiveCachePtr>(q);

	_insertCache<SelectiveInfoCache>(NULL);
	_insertCache<BondSmallViewCache>(NULL);
	_insertCache<BondQuoteCache>(NULL);
	_insertCache<BondQuoteReferCache>(NULL);
	_insertCache<BondBestQuoteCache>(NULL);
	_insertCache<BondDealCache>(NULL);
	_insertCache<BondTraderCache>(NULL);
	_insertCache<ChinaBondValuationCache>(NULL);
	_insertCache<IssuingInstitutionCache>(NULL);
	_insertCache<TradingInstitutionCache>(NULL);
	_insertCache<BondGroupCache>(NULL);
	_insertCache<BondGroupDetailCache>(NULL);
	_insertCache<AccountCache>(NULL);
	_insertCache<BondTypeSelectiveCache>(NULL);

	_insertCacheInThreadSafty<SelectiveInfoCache>(NULL);
	_insertCacheInThreadSafty<BondSmallViewCache>(NULL);
	_insertCacheInThreadSafty<BondQuoteCache>(NULL);
	_insertCacheInThreadSafty<BondQuoteReferCache>(NULL);
	_insertCacheInThreadSafty<BondBestQuoteCache>(NULL);
	_insertCacheInThreadSafty<BondDealCache>(NULL);
	_insertCacheInThreadSafty<BondTraderCache>(NULL);
	_insertCacheInThreadSafty<ChinaBondValuationCache>(NULL);
	_insertCacheInThreadSafty<IssuingInstitutionCache>(NULL);
	_insertCacheInThreadSafty<TradingInstitutionCache>(NULL);
	_insertCacheInThreadSafty<BondGroupCache>(NULL);
	_insertCacheInThreadSafty<BondGroupDetailCache>(NULL);
	_insertCacheInThreadSafty<AccountCache>(NULL);
	_insertCacheInThreadSafty<BondTypeSelectiveCache>(NULL);

	_updateCacheByQuery<SelectiveInfoCache>(q);
	_updateCacheByQuery<BondSmallViewCache>(q);
	_updateCacheByQuery<BondQuoteCache>(q);
	_updateCacheByQuery<BondQuoteReferCache>(q);
	_updateCacheByQuery<BondBestQuoteCache>(q);
	_updateCacheByQuery<BondDealCache>(q);
	_updateCacheByQuery<BondTraderCache>(q);
	_updateCacheByQuery<ChinaBondValuationCache>(q);
	_updateCacheByQuery<IssuingInstitutionCache>(q);
	_updateCacheByQuery<TradingInstitutionCache>(q);
    _updateCacheByQuery<BondGroupCache>(q);
    _updateCacheByQuery<BondGroupDetailCache>(q);
	_updateCacheByQuery<AccountCache>(q);
	_updateCacheByQuery<BondTypeSelectiveCache>(q);

	_updateCacheByQueryInThreadSafty<SelectiveInfoCache>(q);
	_updateCacheByQueryInThreadSafty<BondSmallViewCache>(q);
	_updateCacheByQueryInThreadSafty<BondQuoteCache>(q);
	_updateCacheByQueryInThreadSafty<BondQuoteReferCache>(q);
	_updateCacheByQueryInThreadSafty<BondBestQuoteCache>(q);
	_updateCacheByQueryInThreadSafty<BondDealCache>(q);
	_updateCacheByQueryInThreadSafty<BondTraderCache>(q);
	_updateCacheByQueryInThreadSafty<ChinaBondValuationCache>(q);
	_updateCacheByQueryInThreadSafty<IssuingInstitutionCache>(q);
	_updateCacheByQueryInThreadSafty<TradingInstitutionCache>(q);
    _updateCacheByQueryInThreadSafty<BondGroupCache>(q);
    _updateCacheByQueryInThreadSafty<BondGroupDetailCache>(q);
	_updateCacheByQueryInThreadSafty<AccountCache>(q);
	_updateCacheByQueryInThreadSafty<BondTypeSelectiveCache>(q);

	_deleteCacheByQuery<SelectiveInfoCache>(q);
	_deleteCacheByQuery<BondSmallViewCache>(q);
	_deleteCacheByQuery<BondQuoteCache>(q);
	_deleteCacheByQuery<BondQuoteReferCache>(q);
	_deleteCacheByQuery<BondBestQuoteCache>(q);
	_deleteCacheByQuery<BondDealCache>(q);
	_deleteCacheByQuery<BondTraderCache>(q);
	_deleteCacheByQuery<ChinaBondValuationCache>(q);
	_deleteCacheByQuery<IssuingInstitutionCache>(q);
	_deleteCacheByQuery<TradingInstitutionCache>(q);
	_deleteCacheByQuery<BondGroupCache>(q);
	_deleteCacheByQuery<BondGroupDetailCache>(q);
	_deleteCacheByQuery<AccountCache>(q);
	_deleteCacheByQuery<BondTypeSelectiveCache>(q);

	_deleteCacheByQueryInthreadSafty<SelectiveInfoCache>(q);
	_deleteCacheByQueryInthreadSafty<BondSmallViewCache>(q);
	_deleteCacheByQueryInthreadSafty<BondQuoteCache>(q);
	_deleteCacheByQueryInthreadSafty<BondQuoteReferCache>(q);
	_deleteCacheByQueryInthreadSafty<BondBestQuoteCache>(q);
	_deleteCacheByQueryInthreadSafty<BondDealCache>(q);
	_deleteCacheByQueryInthreadSafty<BondTraderCache>(q);
	_deleteCacheByQueryInthreadSafty<ChinaBondValuationCache>(q);
	_deleteCacheByQueryInthreadSafty<IssuingInstitutionCache>(q);
	_deleteCacheByQueryInthreadSafty<TradingInstitutionCache>(q);
	_deleteCacheByQueryInthreadSafty<BondGroupCache>(q);
	_deleteCacheByQueryInthreadSafty<BondGroupDetailCache>(q);
	_deleteCacheByQueryInthreadSafty<AccountCache>(q);
	_deleteCacheByQueryInthreadSafty<BondTypeSelectiveCache>(q);
}

