/*
 * CacheCenter.h
 *
 *  Created on: Jun 3, 2014
 *      Author: theochen
 */

#ifndef CACHECENTER_H_
#define CACHECENTER_H_

#include "cache/model/selectiveInfo_cache.h"
#include "cache/model/bondsmallview_cache.h"
#include "cache/model/bondquote_cache.h"
#include "cache/model/bondquoterefer_cache.h"
#include "cache/model/bondbestquote_cache.h"
#include "cache/model/bonddeal_cache.h"
#include "cache/model/bondtrader_cache.h"
#include "cache/model/chinabondvaluation_cache.h"
#include "cache/model/issuinginstitution_cache.h"
#include "cache/model/tradinginstitution_cache.h"
#include "cache/model/bondgroup_cache.h"
#include "cache/model/bondgroupdetail_cache.h"
#include "cache/model/account_cache.h"
#include "cache/model/bondtypeselective_cache.h"
#include "db/dbconnector.h"
#include "sdbus/connection.h"

class CacheCenter {

private:
    CacheCenter();
    CacheCenter(CacheCenter const& cacheCenter); // never implement
    virtual ~CacheCenter();

    void operator=(CacheCenter const& cacheCenter); // never implement

public:
    static CacheCenter* sharedInstance()
    {
        if (_singleton == NULL)
        {
            _singleton = new CacheCenter();

        }
        return _singleton;
    }

    void CommitCache();
	void RollbackCache();
    void AttachCurrentThread();
    void DetachCurrentThread();

	sdbus::Connection& GetBusConnection();

public:
    static CacheCenter* _singleton;
    dbDatabase _fstDb;
	sdbus::Connection busConn;

};

#endif /* CACHECENTER_H_ */
