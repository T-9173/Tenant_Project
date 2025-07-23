#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "Tenant.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class DatabaseManager {
public:
    DatabaseManager(const std::string& host, const std::string& user, const std::string& password, const std::string& schema);
    ~DatabaseManager();

    void createTableFromUserInput();

    void createNewTable(int numRows);
    std::vector<Tenant> fetchTenants();
    void exportNames(const std::string& table, const std::vector<std::string>& names);

private:
    sql::Driver* driver;
    std::unique_ptr<sql::Connection> con;
};

#endif 