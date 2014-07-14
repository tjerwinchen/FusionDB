/*
 * AccountCacheController.h
 *
 *  Created on: July 08, 2014
 *      Author: theochen
 */

#ifndef ACCOUNT_CACHECONTROLLER_H_
#define ACCOUNT_CACHECONTROLLER_H_

#include "CacheController.h"
#include "cache/model/account_cache.h"

class AccountCacheController : public CacheController {
public:
    AccountCacheController();
    virtual ~AccountCacheController();

	DECLARE_CACHE_CONTROLLER_INTERFACE(AccountCache, AccountCachePtr);

    static void printAll(AccountCache* cache, void* param = NULL);
    void testCaseShowAll();
	void testInsert();
	void testUpdate();
	void testDelete();

private:
	void insertCache(sql::ResultSet* resSet);
	// @return number of effected lines
	int updateDiskDB(void* cacheAfterUpdate, void* cacheBeforeUpdate);
	int insertDiskDB(void* cache);
	int deleteDiskDB(vector<string>& primaryKeyList);	

};

#endif /* ACCOUNT_CACHECONTROLLER_H_ */

