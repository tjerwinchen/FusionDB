/*
 * institution_cachecontroller.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: theochen
 */

#include "../stdafx.h"
#include "issuinginstitution_cachecontroller.h"
#include "CacheCenter.h"


IssuingInstitutionCacheController::IssuingInstitutionCacheController() : CacheController() {
    // TODO Auto-generated constructor stub
    _class_name = "IssuingInstitutionCacheController";
    _sqlTag = "FetchIssuingInstitutionSQL";
}

IssuingInstitutionCacheController::~IssuingInstitutionCacheController() {
    // TODO Auto-generated destructor stub
}

DEFINE_CACHE_CONTROLLER_INTERFACE(IssuingInstitutionCacheController, IssuingInstitutionCache, IssuingInstitutionCachePtr)

void IssuingInstitutionCacheController::insertCache(sql::ResultSet* resSet)
{
    SET_PROPERTIES_BEGIN(IssuingInstitutionCache);
	SET_STRING_FIELD(id);
    SET_STRING_FIELD(institution_code);
    SET_STRING_FIELD(institution_type);
    SET_STRING_FIELD(full_name_c);
	SET_STRING_FIELD(delflag);
    SET_PROPERTIES_END;
}

int IssuingInstitutionCacheController::insertDiskDB(void* cache)
{
	IssuingInstitutionCache* newCache = (IssuingInstitutionCache*)cache;

	INSERT_DATABASE_BEGIN(IssuingInstitutionCache::TABLE_NAME);

	INSERT_STRING_FIELD(id);
	INSERT_STRING_FIELD(institution_code);
	INSERT_STRING_FIELD(institution_type);
	INSERT_STRING_FIELD(full_name_c);
	INSERT_STRING_FIELD(delflag);

	INSERT_DATABASE_END(IssuingInstitutionCache::TABLE_NAME);
}

int IssuingInstitutionCacheController::updateDiskDB(void* cacheAfterUpdate, void* cacheBeforeUpdate) {

	IssuingInstitutionCache* newCache = (IssuingInstitutionCache*)cacheAfterUpdate;
	IssuingInstitutionCache* oldCache = (IssuingInstitutionCache*)cacheBeforeUpdate;
	UPDATE_DATABASE_BEGIN(IssuingInstitutionCache::TABLE_NAME);

	UPDATE_STRING_FIELD(institution_code);
	UPDATE_STRING_FIELD(institution_type);
	UPDATE_STRING_FIELD(full_name_c);
	UPDATE_STRING_FIELD(delflag);

	CONDITION_STRING_FIELD(id);

	UPDATE_DATABASE_END(IssuingInstitutionCache::TABLE_NAME);
}

int IssuingInstitutionCacheController::deleteDiskDB(vector<string>& primaryKeyList)
{
	DELETE_DATABASE_BEGIN(IssuingInstitutionCache::TABLE_NAME);
	DELETE_DATABASE_END(IssuingInstitutionCache::PKEY_NAME);
}

void IssuingInstitutionCacheController::testCaseShowAll()
{
    dbQuery q;
    q = "";
    traverseCacheByQueryInThreadSafty(q, printAll);
}

void IssuingInstitutionCacheController::printAll(IssuingInstitutionCache* cache, void* param)
{
    cout << cache->institution_code << '\t';
    cout << cache->institution_type << '\t';
    cout << cache->full_name_c << endl;
}

void IssuingInstitutionCacheController::testInsert()
{
	IssuingInstitutionCacheController controller;
	dbQuery q;
	q = "id = ", "4028819b45ab17910145ab3b1ef5000f";
	IssuingInstitutionCachePtr cache = controller.getCacheByQueryInThreadSafty(q);
	if (cache != NULL)
	{
		cache->id = "theochen";
		controller.insertCacheInThreadSafty(cache.get());
	}  
}

void updateCallBack(IssuingInstitutionCache* cache, void* param) {
	cache->full_name_c = "theochen";
}

void updateSuccessed(IssuingInstitutionCache* cache, void* param) {
	cout << "Success for updating BondTraderCache: " << cache->id << endl;
}

void updateFailed(IssuingInstitutionCache* cache, void* param) {
	cout << "Failed for updating BondTraderCache: " << cache->id << endl;
}

void IssuingInstitutionCacheController::testUpdate()
{
	IssuingInstitutionCacheController controller;
	dbQuery q;
	q = "id = ", "theochen";

	controller.updateCacheByQueryInThreadSafty(q, updateCallBack, NULL, updateSuccessed, updateFailed);	
}

void IssuingInstitutionCacheController::testDelete()
{
	IssuingInstitutionCacheController controller;
	dbQuery q;
	q = "id = ", "theochen";
	controller.deleteCacheByQueryInThreadSafty(q);	
}
