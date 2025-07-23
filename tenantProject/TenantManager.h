#ifndef TENANT_MANAGER_H
#define TENANT_MANAGER_H

#include "Tenant.h"
#include "Trie.h"
#include <vector>
#include <string>

class TenantManager {
public:
    void addTenant(const Tenant& tenant);
    const std::vector<Tenant>& getTenants() const;

    bool searchByName(const std::string& firstName, const std::string& lastName) const;

    std::vector<Tenant> getTenantsSortedByRent(bool ascending = true);
    std::vector<Tenant> getTenantsWithComplaints() const;
    std::vector<std::string> getAllFirstNames() const;
    std::vector<std::string> getAllLastNames() const;

    double getAverageRent() const;
    double getAverageWaterBill() const;
    double getAverageParkingFee() const;
    double getTotalRevenue() const;
    Tenant getTenantWithMaxFee(const std::string& feeType) const;
    Tenant getTenantWithMinFee(const std::string& feeType) const;

private:
    std::vector<Tenant> tenants;
    Trie firstNameTrie;
    Trie lastNameTrie;
};

#endif 