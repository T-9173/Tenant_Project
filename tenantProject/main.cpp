#include "DatabaseManager.h"
#include "TenantManager.h"
#include "UIManager.h"
#include <iostream>
#include <cstdlib> 
#include <stdexcept>
#include <ctime>   


void runApplicationLoop(TenantManager& tenantManager, DatabaseManager& dbManager);

int main() {
    srand(static_cast<unsigned int>(time(0)));

    char* db_password_buffer = nullptr;
    size_t buffer_size = 0;
    if (_dupenv_s(&db_password_buffer, &buffer_size, "MY_SQL_PASSWORD") != 0 || db_password_buffer == nullptr) {
        UIManager::printError("MY_SQL_PASSWORD environment variable not set.");
        return EXIT_FAILURE;
    }
    std::string db_password = db_password_buffer;
    free(db_password_buffer);

    try {
        DatabaseManager dbManager("tcp://127.0.0.1:3306", "root", db_password, "tenantdata");
        TenantManager tenantManager;
        std::vector<Tenant> tenants;

        bool loadSuccessful = false;
        while (!loadSuccessful) {
            bool tableExists = !dbManager.fetchTenants().empty();
            int choice = UIManager::getStartupChoice(tableExists);

            switch (choice) {
            case 1:
                tenants = dbManager.fetchTenants();
                loadSuccessful = true;
                break;
            case 2: 
                dbManager.createTableFromUserInput();
                tenants = dbManager.fetchTenants(); 
                loadSuccessful = true;
                break;
            case 3: 
            {
                int rows = UIManager::getValidatedInt("Enter number of rows to generate: ");
                dbManager.createNewTable(rows);
                tenants = dbManager.fetchTenants(); 
                loadSuccessful = true;
            }
            break;
            case 0: 
                UIManager::printMessage("Exiting program.");
                return EXIT_SUCCESS;
            default:
                UIManager::printError("Invalid choice, please try again.");
                break;
            }
        }

        for (const auto& tenant : tenants) {
            tenantManager.addTenant(tenant);
        }
        UIManager::printMessage("\n" + std::to_string(tenantManager.getTenants().size()) + " tenants loaded successfully. Starting application...");

        runApplicationLoop(tenantManager, dbManager);

    }
    catch (const std::exception& e) {
        UIManager::printError("A critical error occurred: " + std::string(e.what()));
        return EXIT_FAILURE;
    }

    UIManager::printMessage("Exiting program.");
    return EXIT_SUCCESS;
}

void runApplicationLoop(TenantManager& tenantManager, DatabaseManager& dbManager) {
    double totalInvestment = UIManager::getValidatedDouble("Enter total property investment cost: $");
    double monthlyCosts = UIManager::getValidatedDouble("Enter total monthly property costs: $");

    bool running = true;
    while (running) {
        int choice = UIManager::getMainMenuChoice();
        UIManager::clearScreen();

        switch (choice) {
        case 1:
            UIManager::printMessage("Average Rent: $" + std::to_string(tenantManager.getAverageRent()));
            break;
        case 2: {
            std::string feeType = UIManager::getFeeTypeChoice();
            Tenant t = tenantManager.getTenantWithMaxFee(feeType);
            UIManager::printMessage("Tenant paying the most for " + feeType + ":");
            UIManager::printTenantDetails(t);
            break;
        }
        case 3: {
            std::string feeType = UIManager::getFeeTypeChoice();
            Tenant t = tenantManager.getTenantWithMinFee(feeType);
            UIManager::printMessage("Tenant paying the least for " + feeType + ":");
            UIManager::printTenantDetails(t);
            break;
        }
        case 4:
            UIManager::printAllTenants(tenantManager.getTenantsSortedByRent(true));
            break;
        case 5:
            UIManager::printAllTenants(tenantManager.getTenantsSortedByRent(false));
            break;
        case 6:
            UIManager::printMessage("Average Water Bill: $" + std::to_string(tenantManager.getAverageWaterBill()));
            break;
        case 7:
            UIManager::printMessage("Average Parking Fee: $" + std::to_string(tenantManager.getAverageParkingFee()));
            break;
        case 8: {
            dbManager.exportNames("tenant_first_names", tenantManager.getAllFirstNames());
            dbManager.exportNames("tenant_last_names", tenantManager.getAllLastNames());
            UIManager::printMessage("First and last names exported to new database tables.");
            break;
        }
        case 9: {
            std::string fname = UIManager::getValidatedString("Enter first name: ");
            std::string lname = UIManager::getValidatedString("Enter last name: ");
            if (tenantManager.searchByName(fname, lname)) {
                UIManager::printMessage(fname + " " + lname + " found in the system.");
            }
            else {
                UIManager::printMessage(fname + " " + lname + " not found.");
            }
            break;
        }
        case 10: {
            double totalRevenue = tenantManager.getTotalRevenue();
            double net = totalRevenue - monthlyCosts;
            UIManager::printMessage("Total monthly revenue (rent + bills): $" + std::to_string(totalRevenue));
            UIManager::printMessage("Net monthly income (after costs): $" + std::to_string(net));
            if (net > 0) {
                double monthsToBreakEven = (totalInvestment) / net;
                UIManager::printMessage("At this rate, it will take " + std::to_string(monthsToBreakEven) + " months to break even on your investment.");
            }
            else {
                UIManager::printMessage("You are currently losing money each month.");
            }
            break;
        }
        case 11:
            UIManager::printAllTenants(tenantManager.getTenantsWithComplaints());
            break;
        case 12:
            UIManager::printAllTenants(tenantManager.getTenants());
            break;
        case 0:
            running = false;
            break;
        }
        if (running) {
            UIManager::printMessage("\nPress Enter to continue...");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
}