/*
* account_cache.h
*
*  Created on: July 08, 2014
*      Author: theochen
*/

#ifndef ACCOUNT_CACHE_H_
#define ACCOUNT_CACHE_H_

class AccountCache;
typedef std::tr1::shared_ptr<AccountCache> AccountCachePtr;

class AccountCache {
public:
	static string SCHEMA_NAME;
	static string TABLE_NAME;
	static string PKEY_NAME;
public:
	AccountCache();
	virtual ~AccountCache();
	string getPrimaryKey();

	string id;
	string company_id;
	string company_name;
	string account_code;
	string username;
	string password;
	string init_password;
	string display_name;
	string mobile;
	string telephone;
	string email;
	string rm;
	string msn;
	string qq;
	string address;
	string account_type;
	string is_forbidden;
	string create_time;
	string modify_time;
	string create_by;
	string modify_by;
	string is_activate;
	string status;
	string cn_name;
	string cn_first_name;
	string en_name;
	string en_first_name;
	string sex;
	string ctiy; // typo in database, we keep it consistent to database
	string dept;
	string title;

	//Non-Dabase Fields

public:
	TYPE_DESCRIPTOR(
		(KEY(id, HASHED | INDEXED),
		FIELD(company_id),
		FIELD(company_name),
		KEY(account_code, HASHED | INDEXED),
		KEY(username, HASHED | INDEXED),
		FIELD(password),
		FIELD(init_password),
		FIELD(display_name),
		FIELD(mobile),
		FIELD(telephone),
		FIELD(email),
		FIELD(rm),
		FIELD(msn),
		FIELD(qq),
		FIELD(address),
		FIELD(account_type),
		FIELD(is_forbidden),
		FIELD(create_time),
		FIELD(modify_time),
		FIELD(create_by),
		FIELD(modify_by),
		FIELD(is_activate),
		FIELD(status),
		FIELD(cn_name),
		FIELD(cn_first_name),
		FIELD(en_name),
		FIELD(en_first_name),
		FIELD(sex),
		FIELD(ctiy), // typo in database, we keep it consistent to database 
		FIELD(dept),
		FIELD(title)
		)
		);
};

#endif /* BONDBESTQUOTE_CACHE_H_ */
