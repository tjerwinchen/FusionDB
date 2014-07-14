#include "dbconnector.h"
#include "include/stdafx.h"

DBConnector* DBConnector::connector_ = 0;

DBConnector* DBConnector::getConnector()
{
    if (connector_ == NULL) {
        connector_ = new DBConnector();
    }

    return connector_;
}

DBConnector::DBConnector()
    : driver_(0), conn_(0)
{
	connectWithDBSchema("tcp://192.168.1.102:3306","artogrid", "artogrid", SCHEMA_IDB_BOND);  
}

DBConnector::~DBConnector()
{
	delete conn_;
}

std::string DBConnector::getSchema(){
	return conn_->getSchema();
}

bool DBConnector::setSchema(std::string schema) {
	try {
		std::string schema_in_use = conn_->getSchema();
		if (schema_in_use != schema)
		{
			conn_->setSchema(schema);
		}
		return true;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in "  << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	return false;
}

bool DBConnector::connectWithDBSchema(string hostwithport, string username, string passwd, string schema)
{
	try {
		// Create a connection
		driver_ = sql::mysql::get_mysql_driver_instance();
		conn_ = driver_->connect(hostwithport, username, passwd);
		conn_->setSchema(schema);
		return true;
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in "  << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	return false;
}

sql::ResultSet* DBConnector::executeQuery(string sqlStr)
{
	sql::Statement* stmt = NULL;
	sql::ResultSet* resSet = NULL;
	try
	{
		stmt = conn_->createStatement();
		resSet = stmt->executeQuery(sqlStr);
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in "  << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	if (stmt != NULL)
	{
		delete stmt;
	}
	return resSet;
}

bool DBConnector::execute(std::string sqlStr)
{
	sql::Statement* stmt = NULL;
	sql::ResultSet* resSet = NULL;
	m._Lock();
	bool retResult = false;
	try
	{
		stmt = conn_->createStatement(); 
		retResult = stmt->execute(sqlStr);
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in "  << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	if (stmt != NULL)
	{
		delete stmt;
	}
	m._Unlock();
	return retResult;
}

int DBConnector::executeUpdate(std::string sqlStr)
{			  
	sql::Statement* stmt = NULL;
	sql::ResultSet* resSet = NULL;
	m._Lock();
	int effectedLines = -1;
	try
	{
		stmt = conn_->createStatement(); 
		effectedLines = stmt->executeUpdate(sqlStr);
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in "  << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	if (stmt != NULL)
	{
		delete stmt;
	}
	m._Unlock();
	return effectedLines;
}