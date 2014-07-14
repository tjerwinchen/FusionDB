/*
 * account_cache.cpp
 *
 *  Created on: July 08, 2014
 *      Author: theochen
 */
#include "stdafx.h"
#include "account_cache.h"

string AccountCache::SCHEMA_NAME = SCHEMA_IDB_ACCOUNT;
string AccountCache::TABLE_NAME = "idb_account";
string AccountCache::PKEY_NAME = "id";

AccountCache::AccountCache() {
    // TODO Auto-generated constructor stub
}

string AccountCache::getPrimaryKey() {
	return id;
}

AccountCache::~AccountCache() {
    // TODO Auto-generated destructor stub
}

REGISTER(AccountCache);
