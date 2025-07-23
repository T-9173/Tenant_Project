#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <string>
#include <vector>
#include "Tenant.h"
#include <cppconn/exception.h>

class UIManager {
public:
    static void clearScreen();
    static void printMessage(const std::string& message);
    static void printError(const std::string& error);
    static void printError(const sql::SQLException& err);

    static std::string getValidatedString(const std::string& prompt);
    static double getValidatedDouble(const std::string& prompt);
    static int getValidatedInt(const std::string& prompt);
    static bool getValidatedBool(const std::string& prompt);

    static int getStartupChoice(bool tableExists);

    static int getMainMenuChoice();
    static void printTenantDetails(const Tenant& tenant);
    static void printAllTenants(const std::vector<Tenant>& tenants);
    static std::string getFeeTypeChoice();
};

#endif 