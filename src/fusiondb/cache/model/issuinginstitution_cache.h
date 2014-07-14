/*
 * issuinginstitution_cache.h
 *
 *  Created on: June 30, 2014
 *      Author: theochen
 */

#ifndef ISSUINGINSTITUTION_CACHE_H_
#define ISSUINGINSTITUTION_CACHE_H_

#include "include/stdafx.h"

class IssuingInstitutionCache;
typedef std::tr1::shared_ptr<IssuingInstitutionCache> IssuingInstitutionCachePtr;

class IssuingInstitutionCache {
public:
	static string SCHEMA_NAME;
	static string TABLE_NAME;
	static string PKEY_NAME;
public:
    IssuingInstitutionCache();
    virtual ~IssuingInstitutionCache();
	string getPrimaryKey();

public:
	string id;
    string institution_code;
    string institution_type;
    string full_name_c;
	string delflag;

public:
    inline IssuingInstitutionCache& operator == (const IssuingInstitutionCache& cache) {
          if (&cache == this)
          {
              return *this;
          }
          this->institution_code = cache.institution_code;
          this->institution_type = cache.institution_type;
          this->full_name_c = cache.full_name_c;

          return *this;
    }

public:
    TYPE_DESCRIPTOR(
        (KEY(id, HASHED | INDEXED),
		 KEY(institution_code, HASHED | INDEXED),
         FIELD(institution_type),
         FIELD(full_name_c),
		 FIELD(delflag)
        )
    );
};


/*
#define KEY(x, index) \
    (*FASTDB_NS::dbDescribeField(new FASTDB_NS::dbFieldDescriptor(#x, (char*)&x-(char*)this, \
                                                                  sizeof(x), index), x))
                                                                  */

#endif /* ISSUINGINSTITUTION_CACHE_H_ */
