#include "DatabaseManager.h"
#include "UIManager.h" 
#include <stdexcept>
#include <cstdlib>
#include <ctime>

std::string randomString() {
    char strings[] = "abcdefghijklmnopqrstuvwxyz";
    std::string result = "";
    for (int i = 0; i < 5; i++) {
        result += strings[rand() % 26];
    }
    return result;
}

DatabaseManager::DatabaseManager(const std::string& host, const std::string& user, const std::string& password, const std::string& schema) {
    try {
        driver = get_driver_instance();
        con.reset(driver->connect(host, user, password));
        con->setSchema(schema);
    }
    catch (sql::SQLException& e) {
        UIManager::printError(e);
        throw; 
    }
}

DatabaseManager::~DatabaseManager() {

}

std::vector<Tenant> DatabaseManager::fetchTenants() {
    std::vector<Tenant> tenants;
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT * FROM tenantinfo"));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            Tenant t;
            t.firstName = res->getString("FirstName");
            t.lastName = res->getString("LastName");
            t.rent = res->getDouble("Rent");
            t.hasComplaints = res->getBoolean("Complaints");
            t.waterBill = res->getDouble("WaterBill");
            t.parkingFee = res->getDouble("ParkingFee");
            tenants.push_back(t);
        }
    }
    catch (sql::SQLException& e) {

    }
    return tenants;
}

void DatabaseManager::createNewTable(int numRows) {
    try {
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("DROP TABLE IF EXISTS tenantinfo");
        stmt->execute("CREATE TABLE tenantinfo(FirstName VARCHAR(255), LastName VARCHAR(255), Rent DOUBLE, Complaints TINYINT, WaterBill DOUBLE, ParkingFee DOUBLE)");

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO tenantinfo(FirstName, LastName, Rent, Complaints, WaterBill, ParkingFee) VALUES (?, ?, ?, ?, ?, ?)"));
        double park[] = { 0, 40, 80, 120 };

        for (int i = 0; i < numRows; ++i) {
            pstmt->setString(1, randomString());
            pstmt->setString(2, randomString());
            pstmt->setDouble(3, (rand() % 2501) + 500.0);
            pstmt->setBoolean(4, (i % 100 == 0)); 
            pstmt->setDouble(5, rand() % 120 + 10);
            pstmt->setDouble(6, park[rand() % 4]);
            pstmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        UIManager::printError(e);
    }
}

void DatabaseManager::createTableFromUserInput() {
    try {
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("DROP TABLE IF EXISTS tenantinfo");
        stmt->execute("CREATE TABLE tenantinfo(FirstName VARCHAR(255), LastName VARCHAR(255), Rent DOUBLE, Complaints TINYINT, WaterBill DOUBLE, ParkingFee DOUBLE)");

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO tenantinfo(FirstName, LastName, Rent, Complaints, WaterBill, ParkingFee) VALUES (?, ?, ?, ?, ?, ?)"));

        int numTenants = UIManager::getValidatedInt("How many tenants would you like to enter? ");
        for (int i = 0; i < numTenants; ++i) {
            UIManager::printMessage("\nEntering data for Tenant #" + std::to_string(i + 1));

            std::string fName = UIManager::getValidatedString("Enter first name: ");
            std::string lName = UIManager::getValidatedString("Enter last name: ");
            double rent = UIManager::getValidatedDouble("Enter rent: $");
            bool complaints = UIManager::getValidatedBool("Have there been complaints (true/false)? ");
            double waterBill = UIManager::getValidatedDouble("Enter water bill: $");
            double parkingFee = UIManager::getValidatedDouble("Enter parking fee: $");

            pstmt->setString(1, fName);
            pstmt->setString(2, lName);
            pstmt->setDouble(3, rent);
            pstmt->setBoolean(4, complaints);
            pstmt->setDouble(5, waterBill);
            pstmt->setDouble(6, parkingFee);
            pstmt->executeUpdate();
        }
        UIManager::printMessage("Successfully added " + std::to_string(numTenants) + " tenants to the database.");

    }
    catch (sql::SQLException& e) {
        UIManager::printError(e);
    }
}


void DatabaseManager::exportNames(const std::string& table, const std::vector<std::string>& names) {
    try {
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("DROP TABLE IF EXISTS " + table);
        stmt->execute("CREATE TABLE " + table + "(Name VARCHAR(255))");

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO " + table + "(Name) VALUES (?)"));
        for (const auto& name : names) {
            pstmt->setString(1, name);
            pstmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        UIManager::printError(e);
    }
}