/*
 * CacheCenter.cpp
 *
 *  Created on: Jun 3, 2014
 *      Author: theochen
 */

#include "../stdafx.h"
#include "CacheCenter.h"

CacheCenter* CacheCenter::_singleton = NULL;

CacheCenter::CacheCenter() {
    // TODO Auto-generated constructor stub

    dbDatabase::OpenParameters params;

    params.databaseName = _T("BCCache");
    params.accessType = dbDatabase::dbAllAccess;
    params.waitLockTimeoutMsec = 1;

	if (!busConn.Connect()) {
		cout << "Fail to connect SDBUS" << endl;
	}	

    if (!_fstDb.open(params)) {
        cout << "Fail to open fast database" << endl;
    }
    else
    {
        cout << "Fast database open" << endl;
    }
}

sdbus::Connection& CacheCenter::GetBusConnection() {
	return busConn;
}

void CacheCenter::AttachCurrentThread() {
    _fstDb.attach();
}

void CacheCenter::DetachCurrentThread() {
    _fstDb.detach();
}

CacheCenter::~CacheCenter() {
    _fstDb.close();
    cout << "Fast database Close" << endl;
}

void CacheCenter::CommitCache() {
    try {
        _fstDb.commit();
    }
    catch (dbException e) {
        cout << "Error in Commit - \n" << e.getMsg() << endl;
    }
}

void CacheCenter::RollbackCache() {
	try {
		_fstDb.rollback();
	}
	catch (dbException e) {
		cout << "Error in Rollback - \n" << e.getMsg() << endl;
	}
}




