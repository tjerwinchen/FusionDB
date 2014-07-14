/*
 * issuinginstitution_cache.cpp
 *
 *  Created on: May 30, 2014
 *      Author: theochen
 */

#include "../../stdafx.h"
#include "issuinginstitution_cache.h"

string IssuingInstitutionCache::SCHEMA_NAME = SCHEMA_IDB_BOND;
string IssuingInstitutionCache::TABLE_NAME = "institution";
string IssuingInstitutionCache::PKEY_NAME  = "id";

IssuingInstitutionCache::IssuingInstitutionCache() {
    // TODO Auto-generated constructor stub
	delflag = "0";
}

IssuingInstitutionCache::~IssuingInstitutionCache() {
    // TODO Auto-generated destructor stub
}

string IssuingInstitutionCache::getPrimaryKey() {
	return id;
}


REGISTER(IssuingInstitutionCache);




