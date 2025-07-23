#include "UIManager.h"
#include <iostream>
#include <limits>
#include <algorithm>

void UIManager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void UIManager::printMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void UIManager::printError(const std::string& error) {
    std::cerr << "[ERROR] " << error << std::endl;
}

void UIManager::printError(const sql::SQLException& err) {
    std::cerr << "# ERR: SQLException in " << err.getSQLState() << std::endl;
    std::cerr << "# ERR: " << err.what();
    std::cerr << " (MySQL error code: " << err.getErrorCode() << ")" << std::endl;
}

std::string UIManager::getValidatedString(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::cin >> input;
    while (std::any_of(input.begin(), input.end(), ::isdigit)) {
        printError("Invalid input. Please use letters only.");
        std::cout << prompt;
        std::cin >> input;
    }
    return input;
}

double UIManager::getValidatedDouble(const std::string& prompt) {
    double value;
    std::cout << prompt;
    std::cin >> value;

    while (std::cin.fail() || value < 0) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printError("Invalid input. Please enter a number.");
        }
        else if (value < 0) {
            printError("Invalid input. Please enter a positive number or zero.");
        }

        std::cout << prompt;
        std::cin >> value;
    }
    return value;
}

int UIManager::getValidatedInt(const std::string& prompt) {
    int value;
    std::cout << prompt;
    std::cin >> value;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Invalid input. Please enter a whole number.");
        std::cout << prompt;
        std::cin >> value;
    }
    return value;
}

bool UIManager::getValidatedBool(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::cin >> input;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    while (input != "true" && input != "false") {
        printError("Invalid input. Please enter 'true' or 'false'.");
        std::cout << prompt;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    }
    return input == "true";
}

int UIManager::getStartupChoice(bool tableExists) {
    UIManager::printMessage("\n--- Welcome to the Tenant Management System ---");

    if (tableExists) {
        UIManager::printMessage("1. Load existing tenant data from the database");
    }
    else {
        UIManager::printMessage("1. (No existing data found)");
    }

    UIManager::printMessage("2. Create new tenant list from user input");
    UIManager::printMessage("3. Create new tenant list with random data");
    UIManager::printMessage("0. Exit");

    int choice = getValidatedInt("\nPlease make a selection: ");

    if (choice == 1 && !tableExists) {
        printError("No data to load. Please choose option 2 or 3 to begin.");
        return getStartupChoice(tableExists); 
    }

    return choice;
}

void UIManager::printTenantDetails(const Tenant& tenant) {
    std::cout << "------------------------\n"
        << "First Name: " << tenant.firstName << "\n"
        << "Last Name: " << tenant.lastName << "\n"
        << "Rent: $" << tenant.rent << "\n"
        << "Complaints: " << (tenant.hasComplaints ? "Yes" : "No") << "\n"
        << "Water Bill: $" << tenant.waterBill << "\n"
        << "Parking Fee: $" << tenant.parkingFee << "\n"
        << "------------------------\n";
}

void UIManager::printAllTenants(const std::vector<Tenant>& tenants) {
    if (tenants.empty()) {
        printMessage("No tenants to display.");
        return;
    }
    for (const auto& tenant : tenants) {
        printTenantDetails(tenant);
    }
}

std::string UIManager::getFeeTypeChoice() {
    printMessage("Select fee type:");
    printMessage("1. Rent");
    printMessage("2. Water Bill");
    printMessage("3. Parking Fee");
    int choice = getValidatedInt("> "); 
    switch (choice) {
    case 1: return "rent";
    case 2: return "waterBill";
    case 3: return "parkingFee";
    default:
        printError("Invalid choice, defaulting to Rent.");
        return "rent";
    }
}

int UIManager::getMainMenuChoice() {
    std::cout << "\n--- Tenant Management System ---\n"
        << "1. Show Average Rent\n"
        << "2. Find Tenant Paying Most\n"
        << "3. Find Tenant Paying Least\n"
        << "4. Sort Tenants by Rent (Ascending)\n"
        << "5. Sort Tenants by Rent (Descending)\n"
        << "6. Show Average Water Bill\n"
        << "7. Show Average Parking Fee\n"
        << "8. Export All Names to Database\n"
        << "9. Search for a Tenant\n"
        << "10. Check Investment Break-Even Point\n"
        << "11. List Tenants with Complaints\n"
        << "12. List All Tenants\n"
        << "0. Exit\n"
        << "---------------------------------\n";

    return getValidatedInt("Enter your choice: "); 
}