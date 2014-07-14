/*
 * issuinginstitution_cachecontroller.h
 *
 *  Created on: Jun 11, 2014
 *      Author: theochen
 */

#ifndef ISSUINGINSTITUTION_CACHECONTROLLER_H_
#define ISSUINGINSTITUTION_CACHECONTROLLER_H_

#include "CacheController.h"

class IssuingInstitutionCacheController : public CacheController {
public:
    IssuingInstitutionCacheController();
    virtual ~IssuingInstitutionCacheController();

	DECLARE_CACHE_CONTROLLER_INTERFACE(IssuingInstitutionCache, IssuingInstitutionCachePtr);

public: // Test Methods
    static void printAll(IssuingInstitutionCache* cache, void* param = NULL);
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

#endif /* ISSUINGINSTITUTION_CACHECONTROLLER_H_ */
