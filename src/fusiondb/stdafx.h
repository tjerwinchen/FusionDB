// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <stddef.h>
#include <stdio.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "pugixml/pugixml.hpp"
#include <iostream>
#include <fastdb.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
using namespace std;

#define WINDOWS_PLATFORM  1

const string SCHEMA_IDB_ACCOUNT("idb_account_test");
const string SCHEMA_IDB_BOND("idb_bond_test");

char* allocString(string value);
void stringSetter(const char** property, string value);
char const* stringGetter(char const* property);
void clearStringProperty(const char** property);


#define SET_PROPERTIES_BEGIN(TableName)\
    TableName cache;\

#define SET_STRING_FIELD(field_name)\
    cache.field_name = resSet->getString(#field_name);\

#define SET_INT_FIELD(field_name)\
    cache.field_name = resSet->getInt(#field_name);\

#define SET_DOUBLE_FIELD(field_name)\
    cache.field_name = resSet->getDouble(#field_name);\

#define SET_BOOLEAN_FIELD(field_name)\
    cache.field_name = resSet->getDouble(#field_name);\

#define SET_PROPERTIES_END\
    CacheCenter::sharedInstance()->AttachCurrentThread();\
    CacheCenter::sharedInstance()->_fstDb.insert(cache); \
    CacheCenter::sharedInstance()->DetachCurrentThread();\
    


#define INSERT_DATABASE_BEGIN(table_name)\
	vector<string> fieldList;			 \
	vector<string> conditionList;			 \
	string sqlStr = "insert into " + string(table_name) + " set ";\


#define INSERT_STRING_FIELD(field_name)									\
{																		\
	std::ostringstream s;												\
																		\
	s << #field_name << " = " << "\'" << newCache->field_name << "\'";	\
	fieldList.push_back(s.str());										\
}																		\


#define INSERT_DECIMAL_FIELD(field_name)								\
{																		\
	std::ostringstream s;												\
																		\
	s << #field_name << " = " << newCache->field_name;					\
	fieldList.push_back(s.str());										\
}																		\



#define INSERT_INT_FIELD(field_name)								\
	INSERT_DECIMAL_FIELD(field_name)								\


#define INSERT_DOUBLE_FIELD(field_name)								\
	INSERT_DECIMAL_FIELD(field_name)								\


#define INSERT_BOOLEAN_FIELD(field_name)							\
	INSERT_DECIMAL_FIELD(field_name)								\




#define INSERT_DATABASE_END(table_name)         \
	if (fieldList.size() == 0)                 \
	{                                           \
		std::cout << "All fields are identical, no need to update table - [" << table_name << "]" << endl;\
		return 0;                               \
	}                                           \
	string fieldString = join(fieldList, ",");	\
	sqlStr += fieldString;						\
	cout << sqlStr << endl;																\
	return DBConnector::getConnector()->executeUpdate(sqlStr);						    \



#define UPDATE_DATABASE_BEGIN(table_name)\
	vector<string> fieldList;			 \
	vector<string> conditionList;			 \
	string sqlStr = "update " + string(table_name) + " set ";\


#define UPDATE_STRING_FIELD(field_name)									\
if (newCache->field_name != oldCache->field_name)						\
{																		\
	std::ostringstream s;												\
																		\
	s << #field_name << " = " << "\'" << newCache->field_name << "\'";  \
	fieldList.push_back(s.str());										\
}																		\

#define CONDITION_STRING_FIELD(field_name)								\
{																		\
	std::ostringstream s;												\
																		\
	s << #field_name << " = " << "\'" << oldCache->field_name << "\'";  \
	conditionList.push_back(s.str());									\
}																		\



#define UPDATE_DECIMAL_FIELD(field_name)				\
if (newCache->field_name != oldCache->field_name)		\
{														\
	std::ostringstream s;								\
														\
	s << #field_name << "=" << newCache->field_name; 	\
	fieldList.push_back(s.str());						\
}														\



#define UPDATE_DATABASE_END(table_name)         \
    if (fieldList.size() == 0)                 \
    {                                           \
        std::cout << "All fields are identical, no need to update table - [" << table_name << "]" << endl;\
        return 0;                               \
    }                                           \
	string fieldString = join(fieldList, ",");	\
	sqlStr += fieldString;						\
	sqlStr += " Where ";						\
	string conditionString = join(conditionList, " AND ");								\
	sqlStr += conditionString;															\
	cout << sqlStr << endl;																\
	return DBConnector::getConnector()->executeUpdate(sqlStr);						    \


#define UPDATE_INT_FIELD(field_name)\
	UPDATE_DECIMAL_FIELD(field_name) \


#define UPDATE_DOUBLE_FIELD(field_name)\
	UPDATE_DECIMAL_FIELD(field_name)\


#define UPDATE_BOOLEAN_FIELD(field_name)\
	UPDATE_DECIMAL_FIELD(field_name)\


#define DELETE_DATABASE_BEGIN(table_name)\
	vector<string> fieldList;			 \
	vector<string> conditionList;			 \
	string sqlStr = "delete from " + string(table_name) + " ";\


#define DELETE_DATABASE_END(primary_name)         \
	sqlStr += " Where " + primary_name + " in ";						\
	string conditionString = join(primaryKeyList, ",");								\
	sqlStr += "(" + conditionString + ")";															\
	cout << sqlStr << endl;																\
	return DBConnector::getConnector()->executeUpdate(sqlStr);						    \


#define TIME_COST_BEGIN(log_note)\
    clock_t t1, t2; \
    t1 = clock(); \


#define TIME_COST_END(log_note)\
    t2 = clock(); \
    std::cout << log_note << "... - " <<  " Time costing:" << (t2 - t1)*1000.0/CLOCKS_PER_SEC << "ms" << endl;



#define OVERLOAD_ASSIGNMENT_OPERATOR_BEGIN(ClassName)\
inline ClassName& operator = (const ClassName& cache) {      \
    if (&cache == this)                                      \
    {                                                      \
        return *this;                                      \
    }                                                      \
                                

#define OVERLOAD_ASSIGNMENT_OPERATOR_PROPERTY(property_name)\
    this->property_name = cache.property_name; \


#define OVERLOAD_ASSIGNMENT_OPERATOR_END                   \
    return *this;                                           \
}                                                           \


#define SAFE_DELTE_PROPERTY(property_name)    \
if (property_name != NULL)            \
{                                \
    delete property_name;            \
    property_name = NULL;            \
}                                \





/* ---------------------------------------------------
 * Public Cache Controller Interface
 */
#define DECLARE_CACHE_CONTROLLER_INTERFACE(CACHE_NAME, CACHE_PTR)			\
public:																			\
																				\
	bool insertCache(CACHE_NAME* cache);										\
	bool insertCacheInThreadSafty(CACHE_NAME* cache);							\
																			    \
	void updateCacheByQuery(dbQuery& q, 										\
		void(*callBack)(CACHE_NAME*, void* param), 								\
		void* param = NULL,														\
		void(*updateSuccessed)(CACHE_NAME*, void* param) = NULL,				\
		void(*updateFailed)(CACHE_NAME*, void* param) = NULL					\
		);																		\
	void updateCacheByQueryInThreadSafty(dbQuery& q, 							\
		void(*callBack)(CACHE_NAME*, void* param), 								\
		void* param = NULL,														\
		void(*updateSuccessed)(CACHE_NAME*, void* param) = NULL,				\
		void(*updateFailed)(CACHE_NAME*, void* param) = NULL					\
		);																		\
																				\
	bool deleteCacheByQueryInThreadSafty(dbQuery& q);							\
	bool deleteCacheByQuery(dbQuery& q);										\
																				\
	void cacheTable();															\
																				\
	void traverseCacheByQueryInThreadSafty(dbQuery& q, 							\
	void(*callBack)(CACHE_NAME*, void* param) = NULL, 							\
	void* param = NULL);														\
	void traverseCacheByQuery(dbQuery& q, 										\
		void(*callBack)(CACHE_NAME*, void* param) = NULL, 						\
		void* param = NULL);													\
																				\
	CACHE_PTR getCacheByQueryInThreadSafty(dbQuery& q);							\
	CACHE_PTR getCacheByQuery(dbQuery& q);										\
																				\
	int getSizeByQuery(dbQuery& q);												\
	int getSizeByQueryInThreadSafty(dbQuery& q);								\
																				\
	std::vector<CACHE_PTR> getCacheListByQueryInThreadSafty(dbQuery& q);		\
	std::vector<CACHE_PTR> getCacheListByQuery(dbQuery& q);						\
																				\




#define DEFINE_CACHE_CONTROLLER_INTERFACE(CACHE_CONTROLLER_NAME, CACHE_NAME, CACHE_PTR)	\
void CACHE_CONTROLLER_NAME::cacheTable()													\
{																							\
	_cacheTable<CACHE_NAME>(sqlFetchCache());												\
}																							\
																							\
int CACHE_CONTROLLER_NAME::getSizeByQuery(dbQuery& q)										\
{																							\
	return _getSizeByQuery<CACHE_NAME>(q);													\
}																							\
																							\
int CACHE_CONTROLLER_NAME::getSizeByQueryInThreadSafty(dbQuery& q)							\
{																							\
	return _getSizeByQueryInThreadSafty<CACHE_NAME>(q);										\
}																							\
																							\
bool CACHE_CONTROLLER_NAME::insertCache(CACHE_NAME* cache) {								\
	return _insertCache<CACHE_NAME>(cache);													\
} 																							\
																							\
bool CACHE_CONTROLLER_NAME::insertCacheInThreadSafty(CACHE_NAME* cache) {					\
	return _insertCacheInThreadSafty<CACHE_NAME>(cache);									\
} 																							\
																							\
void CACHE_CONTROLLER_NAME::updateCacheByQuery(dbQuery& q, 									\
	void(*callBack)(CACHE_NAME*, void* param), 												\
	void* param,																			\
	void(*updateSuccessed)(CACHE_NAME*, void* param),										\
	void(*updateFailed)(CACHE_NAME*, void* param)											\
	) {																						\
		_updateCacheByQuery<CACHE_NAME>(q, callBack, param, updateSuccessed, updateFailed);	\
}																							\
																							\
void CACHE_CONTROLLER_NAME::updateCacheByQueryInThreadSafty(dbQuery& q, 					\
	void(*callBack)(CACHE_NAME*, void* param), 												\
	void* param,																			\
	void(*updateSuccessed)(CACHE_NAME*, void* param),										\
	void(*updateFailed)(CACHE_NAME*, void* param)											\
	) {																						\
		_updateCacheByQueryInThreadSafty<CACHE_NAME>(q, callBack, param, updateSuccessed, updateFailed); \
}																										 \
																										 \
bool CACHE_CONTROLLER_NAME::deleteCacheByQueryInThreadSafty(dbQuery& q)									 \
{																										 \
	return _deleteCacheByQueryInthreadSafty<CACHE_NAME>(q);												 \
}																										 \
																										 \
bool CACHE_CONTROLLER_NAME::deleteCacheByQuery(dbQuery& q)												 \
{																										 \
	return _deleteCacheByQuery<CACHE_NAME>(q);															 \
}																										 \
																										 \
void CACHE_CONTROLLER_NAME::traverseCacheByQueryInThreadSafty(dbQuery& q, void(*callBack)(CACHE_NAME*, void* param), void* param) \
{																										 \
	_traverseCacheByQueryInThreadSafty<CACHE_NAME>(q, callBack, param);									 \
}																										 \
																										 \
void CACHE_CONTROLLER_NAME::traverseCacheByQuery(dbQuery& q, void(*callBack)(CACHE_NAME*, void* param), void* param)\
{																										 \
	_traverseCacheByQuery<CACHE_NAME>(q, callBack, param);												 \
}																										 \
																										 \
CACHE_PTR CACHE_CONTROLLER_NAME::getCacheByQuery(dbQuery& q) {											 \
	CACHE_PTR cache(CacheController::getCacheByQuery<CACHE_NAME>(q));									 \
	return cache;																						 \
}																										 \
																										 \
CACHE_PTR CACHE_CONTROLLER_NAME::getCacheByQueryInThreadSafty(dbQuery& q) {								\
	CACHE_PTR cache(CacheController::getCacheByQueryInThreadSafty<CACHE_NAME>(q));						\
	return cache;																						\
}																										\
																										\
std::vector<CACHE_PTR> CACHE_CONTROLLER_NAME::getCacheListByQueryInThreadSafty(dbQuery& q)				\
{																										\
	return CacheController::getCacheListByQueryInThreadSafty<CACHE_NAME, CACHE_PTR>(q);					\
}																										\
																										\
std::vector<CACHE_PTR> CACHE_CONTROLLER_NAME::getCacheListByQuery(dbQuery& q)							\
{																										\
	return CacheController::getCacheListByQuery<CACHE_NAME, CACHE_PTR>(q);								\
}																										\






vector<string> &split(const string &s, char delim, vector<string> &elems);


vector<string> split(const string &s, char delim);

string join(vector<string>& str_arr, string delim);

using namespace std;




#endif
