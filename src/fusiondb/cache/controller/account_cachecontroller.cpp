/*
 * account_cachecontroller.cpp
 *
 *  Created on: July 08, 2014
 *      Author: theochen
 */

#include "stdafx.h"
#include "account_cachecontroller.h"

AccountCacheController::AccountCacheController() {
	// TODO Auto-generated constructor stub
	_class_name = "AccountCacheController";
	_sqlTag = "FetchAccountSQL";
}

AccountCacheController::~AccountCacheController() {
	// TODO Auto-generated destructor stub
}

DEFINE_CACHE_CONTROLLER_INTERFACE(AccountCacheController, AccountCache, AccountCachePtr)

void AccountCacheController::insertCache(sql::ResultSet* resSet)
{
	SET_PROPERTIES_BEGIN(AccountCache);

	SET_STRING_FIELD(id);
	SET_STRING_FIELD(company_id);
	SET_STRING_FIELD(company_name);
	SET_STRING_FIELD(account_code);
	SET_STRING_FIELD(username);
	SET_STRING_FIELD(password);
	SET_STRING_FIELD(init_password);
	SET_STRING_FIELD(display_name);
	SET_STRING_FIELD(mobile);
	SET_STRING_FIELD(telephone);
	SET_STRING_FIELD(email);
	SET_STRING_FIELD(rm);
	SET_STRING_FIELD(msn);
	SET_STRING_FIELD(qq);
	SET_STRING_FIELD(address);
	SET_STRING_FIELD(account_type);
	SET_STRING_FIELD(is_forbidden);
	SET_STRING_FIELD(create_time);
	SET_STRING_FIELD(modify_time);
	SET_STRING_FIELD(create_by);
	SET_STRING_FIELD(modify_by);
	SET_STRING_FIELD(is_activate);
	SET_STRING_FIELD(status);
	SET_STRING_FIELD(cn_name);
	SET_STRING_FIELD(cn_first_name);
	SET_STRING_FIELD(en_name);
	SET_STRING_FIELD(en_first_name);
	SET_STRING_FIELD(sex);
	SET_STRING_FIELD(ctiy);
	SET_STRING_FIELD(dept);
	SET_STRING_FIELD(title);

	SET_PROPERTIES_END;
}

int AccountCacheController::insertDiskDB(void* cache)
{
	AccountCache* newCache = (AccountCache*)cache;

	INSERT_DATABASE_BEGIN(AccountCache::TABLE_NAME);

	INSERT_STRING_FIELD(id);
	INSERT_STRING_FIELD(company_id);
	INSERT_STRING_FIELD(company_name);
	INSERT_STRING_FIELD(account_code);
	INSERT_STRING_FIELD(username);
	INSERT_STRING_FIELD(password);
	INSERT_STRING_FIELD(init_password);
	INSERT_STRING_FIELD(display_name);
	INSERT_STRING_FIELD(mobile);
	INSERT_STRING_FIELD(telephone);
	INSERT_STRING_FIELD(email);
	INSERT_STRING_FIELD(rm);
	INSERT_STRING_FIELD(msn);
	INSERT_STRING_FIELD(qq);
	INSERT_STRING_FIELD(address);
	INSERT_STRING_FIELD(account_type);
	INSERT_STRING_FIELD(is_forbidden);
	INSERT_STRING_FIELD(create_time);
	INSERT_STRING_FIELD(modify_time);
	INSERT_STRING_FIELD(create_by);
	INSERT_STRING_FIELD(modify_by);
	INSERT_STRING_FIELD(is_activate);
	INSERT_STRING_FIELD(status);
	INSERT_STRING_FIELD(cn_name);
	INSERT_STRING_FIELD(cn_first_name);
	INSERT_STRING_FIELD(en_name);
	INSERT_STRING_FIELD(en_first_name);
	INSERT_STRING_FIELD(sex);
	INSERT_STRING_FIELD(ctiy);
	INSERT_STRING_FIELD(dept);
	INSERT_STRING_FIELD(title);

	INSERT_DATABASE_END(AccountCache::TABLE_NAME);
}

int AccountCacheController::updateDiskDB(void* cacheAfterUpdate, void* cacheBeforeUpdate) {

	AccountCache* newCache = (AccountCache*)cacheAfterUpdate;
	AccountCache* oldCache = (AccountCache*)cacheBeforeUpdate;
	UPDATE_DATABASE_BEGIN(AccountCache::TABLE_NAME);
	
	UPDATE_STRING_FIELD(company_id);
	UPDATE_STRING_FIELD(company_name);
	UPDATE_STRING_FIELD(account_code);
	UPDATE_STRING_FIELD(username);
	UPDATE_STRING_FIELD(password);
	UPDATE_STRING_FIELD(init_password);
	UPDATE_STRING_FIELD(display_name);
	UPDATE_STRING_FIELD(mobile);
	UPDATE_STRING_FIELD(telephone);
	UPDATE_STRING_FIELD(email);
	UPDATE_STRING_FIELD(rm);
	UPDATE_STRING_FIELD(msn);
	UPDATE_STRING_FIELD(qq);
	UPDATE_STRING_FIELD(address);
	UPDATE_STRING_FIELD(account_type);
	UPDATE_STRING_FIELD(is_forbidden);
	UPDATE_STRING_FIELD(create_time);
	UPDATE_STRING_FIELD(modify_time);
	UPDATE_STRING_FIELD(create_by);
	UPDATE_STRING_FIELD(modify_by);
	UPDATE_STRING_FIELD(is_activate);
	UPDATE_STRING_FIELD(status);
	UPDATE_STRING_FIELD(cn_name);
	UPDATE_STRING_FIELD(cn_first_name);
	UPDATE_STRING_FIELD(en_name);
	UPDATE_STRING_FIELD(en_first_name);
	UPDATE_STRING_FIELD(sex);
	UPDATE_STRING_FIELD(ctiy);
	UPDATE_STRING_FIELD(dept);
	UPDATE_STRING_FIELD(title);

	CONDITION_STRING_FIELD(id);

	UPDATE_DATABASE_END(AccountCache::TABLE_NAME);
}

int AccountCacheController::deleteDiskDB(vector<string>& primaryKeyList)
{
	DELETE_DATABASE_BEGIN(AccountCache::TABLE_NAME);
	DELETE_DATABASE_END(AccountCache::PKEY_NAME);
}

void AccountCacheController::printAll(AccountCache* cache, void* param)
{
	cout << cache->account_code << '\t';
	cout << cache->username << '\t';
	cout << endl;
}

void AccountCacheController::testCaseShowAll()
{
	dbQuery q;
	q = "";
	traverseCacheByQueryInThreadSafty(q, printAll);
}

void AccountCacheController::testInsert()
{
	AccountCacheController controller;
	dbQuery q;
	q = "id = ", "4028819b4370cce5014370cd08130b66";
	AccountCachePtr bondBestQuote = controller.getCacheByQueryInThreadSafty(q);
	if (bondBestQuote != NULL)
	{
		bondBestQuote->id = "theochen";
		controller.insertCacheInThreadSafty(bondBestQuote.get());
	}  
}

void updateCallBack(AccountCache* cache, void* param) {
	cache->username = "theochen";
}

void updateSuccessed(AccountCache* cache, void* param) {
	cout << "Success for updating AccountCache: " << cache->id << endl;
}

void updateFailed(AccountCache* cache, void* param) {
	cout << "Failed for updating AccountCache: " << cache->id << endl;
}

void AccountCacheController::testUpdate()
{
	AccountCacheController controller;
	dbQuery q;
	q = "id = ", "theochen";

	controller.updateCacheByQueryInThreadSafty(q, updateCallBack, NULL, updateSuccessed, updateFailed);	
}

void AccountCacheController::testDelete()
{
	AccountCacheController controller;
	dbQuery q;
	q = "id = ", "theochen";
	controller.deleteCacheByQueryInThreadSafty(q);	
}
