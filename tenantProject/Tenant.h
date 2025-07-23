#ifndef TENANT_H
#define TENANT_H

#include <string>


struct Tenant {
    std::string firstName;
    std::string lastName;
    double rent = 0.0;
    bool hasComplaints = false;
    double waterBill = 0.0;
    double parkingFee = 0.0;
};

#endif 