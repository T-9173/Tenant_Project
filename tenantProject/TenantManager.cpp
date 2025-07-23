#include "TenantManager.h"
#include <numeric>
#include <algorithm>
#include <limits>
#include <iterator> 

void TenantManager::addTenant(const Tenant& tenant) {
    tenants.push_back(tenant);
    firstNameTrie.insert(tenant.firstName);
    lastNameTrie.insert(tenant.lastName);
}

const std::vector<Tenant>& TenantManager::getTenants() const {
    return tenants;
}

bool TenantManager::searchByName(const std::string& firstName, const std::string& lastName) const {
    return firstNameTrie.search(firstName) && lastNameTrie.search(lastName);
}

std::vector<Tenant> TenantManager::getTenantsSortedByRent(bool ascending) {
    std::vector<Tenant> sortedTenants = tenants;
    std::sort(sortedTenants.begin(), sortedTenants.end(), [ascending](const Tenant& a, const Tenant& b) {
        if (ascending) {
            return a.rent < b.rent;
        }
        return a.rent > b.rent;
        });
    return sortedTenants;
}

std::vector<Tenant> TenantManager::getTenantsWithComplaints() const {
    std::vector<Tenant> tenantsWithComplaints;
    std::copy_if(tenants.begin(), tenants.end(), std::back_inserter(tenantsWithComplaints),
        [](const Tenant& tenant) {
            return tenant.hasComplaints;
        });
    return tenantsWithComplaints;
}

std::vector<std::string> TenantManager::getAllFirstNames() const {
    return firstNameTrie.getAllWords();
}

std::vector<std::string> TenantManager::getAllLastNames() const {
    return lastNameTrie.getAllWords();
}

double TenantManager::getAverageRent() const {
    if (tenants.empty()) return 0.0;
    double total = std::accumulate(tenants.begin(), tenants.end(), 0.0,
        [](double sum, const Tenant& t) { return sum + t.rent; });
    return total / tenants.size();
}

double TenantManager::getAverageWaterBill() const {
    if (tenants.empty()) return 0.0;
    double total = std::accumulate(tenants.begin(), tenants.end(), 0.0,
        [](double sum, const Tenant& t) { return sum + t.waterBill; });
    return total / tenants.size();
}

double TenantManager::getAverageParkingFee() const {
    if (tenants.empty()) return 0.0;
    double total = std::accumulate(tenants.begin(), tenants.end(), 0.0,
        [](double sum, const Tenant& t) { return sum + t.parkingFee; });
    return total / tenants.size();
}

double TenantManager::getTotalRevenue() const {
    return std::accumulate(tenants.begin(), tenants.end(), 0.0,
        [](double sum, const Tenant& t) {
            return sum + t.rent + t.waterBill + t.parkingFee;
        });
}

Tenant TenantManager::getTenantWithMaxFee(const std::string& feeType) const {
    if (tenants.empty()) return {};
    auto it = std::max_element(tenants.begin(), tenants.end(), [&](const Tenant& a, const Tenant& b) {
        if (feeType == "rent") return a.rent < b.rent;
        if (feeType == "waterBill") return a.waterBill < b.waterBill;
        if (feeType == "parkingFee") return a.parkingFee < b.parkingFee;
        return false;
        });
    return *it;
}

Tenant TenantManager::getTenantWithMinFee(const std::string& feeType) const {
    if (tenants.empty()) return {};
    auto it = std::min_element(tenants.begin(), tenants.end(), [&](const Tenant& a, const Tenant& b) {
        if (feeType == "rent") return a.rent < b.rent;
        if (feeType == "waterBill") return a.waterBill < b.waterBill;
        if (feeType == "parkingFee") return a.parkingFee < b.parkingFee;
        return false;
        });
    return *it;
}