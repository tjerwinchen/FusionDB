#ifndef _DBCONNECTOR_H_
#define _DBCONNECTOR_H_

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
using namespace std;

class DBConnector
{
public:
    static DBConnector* getConnector();

    bool connectWithDBSchema(string hostwithport, string username, string passwd, string schema);

    sql::ResultSet* executeQuery(std::string sqlStr);
    bool execute(std::string sqlStr);
	int executeUpdate(std::string sqlStr);
	bool setSchema(std::string schema);
	std::string getSchema();

private:
    DBConnector();
    ~DBConnector();

    sql::mysql::MySQL_Driver* driver_;
    sql::Connection* conn_;

    static DBConnector* connector_;
    std::_Mutex m;
};

#endif