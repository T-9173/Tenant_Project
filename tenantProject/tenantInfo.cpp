#include "TrieDeclaration.h"

tenantInfo::tenantInfo()
{
    Tenant();
    head = nullptr;
    totalPrice = 0;
    monthlyPrice = 0;
}

void tenantInfo::readAll(info* head1)
{
    if (head1) {
        cout << head1->firstName << " ";
        cout << head1->lastName << " ";
        cout << head1->rent << " ";
        cout << head1->complaints << " ";
        cout << head1->waterBill << " ";
        cout << head1->parkingFee << " \n";
        readAll(head1->next);
    }
}
void tenantInfo::insertInfo(info*& head1, string firstName, string lastName,double rent, bool complaints, double waterBill, double parkingFee) {
    if (head1 == nullptr) {
        head1 = new info(firstName, lastName, rent, complaints, waterBill, parkingFee);
        add(firstName,lastName);
        rentQueue.push(rent);
        parkQueue.push(parkingFee);
        waterQueue.push(waterBill);
    }
    else
        insertInfo(head1->next, firstName, lastName, rent, complaints, waterBill, parkingFee);
}

void tenantInfo::details(info* head1, string firstName, string lastName)
{
    while (head1) {
        if (head1->firstName == firstName
            && (head1->lastName == lastName)) {
            cout << "\nFirst Name: " << head1->firstName << endl;
            cout << "Last Name: " << head1->lastName << endl;
            cout << "Rent: " << head1->rent << endl;
            cout << "Complaints: ";
            if (head1->complaints) { cout << "true"; }
            else { cout << "false"; } cout << endl;
            cout << "WaterBill: $" << head1->waterBill << endl;
            cout << "ParkingFee: $" << head1->parkingFee << endl;
        }
     
        head1 = head1->next;
    }
}

void tenantInfo::getComplaints(info* head1)
{
    while (head1) {
        if (head1->complaints) {
            cout << head1->firstName << " ";
            cout << head1->lastName << " ";
            cout << head1->rent << " ";
            cout << head1->complaints << " ";            
            cout << head1->waterBill << " ";
            cout << head1->parkingFee << " \n";
        }
        head1 = head1->next;
    }
}

void tenantInfo::read()
{
    readAll(head);
}

void tenantInfo::insert(string firstName, string lastName, double rent, bool complaints, double waterBill, double parkingFee)
{
    insertInfo(head, firstName, lastName, rent, complaints, waterBill, parkingFee);
}
double tenantInfo::averageRent() {
    queue <double> experiment = rentQueue;
    double sum = 0;
    double num = rentQueue.size();
    for (int i = 0; i < num; i++) {
        sum = sum + experiment.front();
        experiment.pop();
    }
    return sum / num;
}

double tenantInfo::averageWaterBill()
{
    queue <double> experiment = waterQueue;
    double sum = 0;
    double num = waterQueue.size();
    for (int i = 0; i < num; i++) {
        sum = sum + experiment.front();
        experiment.pop();
    }
    return sum / num;
}

double tenantInfo::averageParkingFee()
{
    queue <double> experiment = parkQueue;
    double sum = 0;
    double num = parkQueue.size();
    for (int i = 0; i < num; i++) {
        sum = sum + experiment.front();
        experiment.pop();
    }
    return sum / num;
}

void tenantInfo::radixSortRentNode(string order)
{
    queue <double> rentQueue1 = rentQueue; info* root = head;

    double max = rentQueue1.front();
    int lim = rentQueue1.size();
    info** array1 = new info*[lim];
    for (int i = 0; i < lim; i++) {
        if (max < rentQueue1.front()) {
            max = rentQueue1.front();
        }
        array1[i] = root;
        root = root->next;
        rentQueue1.pop();
    }

    for (int z = 1; max / z > 0; z *= 10) {
        countSort(array1, lim, z);
    }
    for (int i = 0; i < lim - 1; i++) {
        array1[i]->next = array1[i + 1];
        if (i + 1 == lim - 1) {
             array1[i + 1]->next = nullptr;
        }
    }    
    if (order == "normal") {
        head = array1[0];
    }
    else if (order == "reverse") {
        for (int i = lim - 1; i >= 0; i--) {
            array1[i]->next = array1[i - 1];
            if (i == 0) {
                array1[i]->next = nullptr;
            }
        }
        head = array1[lim - 1]; 
    }
}

void tenantInfo::countSort(info* array1[], int size, int z)
{
    int track[10] = { 0 }; info **output = new info*[size];

    for (int i = 0; i < size; i++)
        track[(int(array1[i]->rent) / z) % 10]++;

    for (int i = 1; i < 10; i++)
        track[i] += track[i - 1];

    for (int i = size - 1; i >= 0; i--) {
        output[track[(int(array1[i]->rent) / z) % 10] - 1] = array1[i];
        track[(int(array1[i]->rent) / z) % 10]--;
    }

    for (int i = 0; i < size; i++)
        array1[i] = output[i];
}

double tenantInfo::getMax(string a)
{
    info* root = head; info* print = head;
    double max = root->rent;
    if (a == "rent") {
        max = root->rent;
        while (root) {
            if (max < root->rent) {
                max = root->rent;
                print = root;
            }
            root = root->next;
        }
    }
    else if (a == "waterBill") {
        max = root->waterBill;
        while (root) {
            if (max < root->waterBill) {
                max = root->waterBill;
                print = root;
            }
            root = root->next;
        }
    }
    else if (a == "parkingFee") {
        max = root->parkingFee;
        while (root) {
            if (max < root->parkingFee) {
                max = root->parkingFee;
                print = root;
            }
            root = root->next;
        }
    }
    cout << print->firstName << " " << print->lastName << " pays the most amount of money for " << a;
    cout << endl;
    return max;
}

double tenantInfo::getMin(string a)
{
    info* root = head; info* print = head;
    double min = root->rent;
    if (a == "rent") {
        min = root->rent;
        while (root) {
            if (min > root->rent) {
                min = root->rent;
                print = root;
            }
            root = root->next;
        }
    }
    else if (a == "waterBill") {
        min = root->waterBill;
        while (root) {
            if (min > root->waterBill){
                min = root->waterBill;
                print = root;
             }
            root = root->next;
        }
    }
    else if (a == "parkingFee") {
        min = root->parkingFee;
        while (root) {
            if (min > root->parkingFee) {
                min = root->parkingFee;
                print = root;
            }
            root = root->next;
        }
    }
    cout << print->firstName << " " << print->lastName << " pays the least amount of money for " << a;
    cout << endl;
    return min;
}

void tenantInfo::setTotalPrice(double price)
{
    totalPrice = price;
}

double tenantInfo::getTotalPrice()
{
    return totalPrice;
}

void tenantInfo::setMonthlyPrice(double price)
{
    monthlyPrice = price;
}

double tenantInfo::getMonthlyPrice()
{
    return monthlyPrice;
}

double tenantInfo::getAllSum()
{
    double sum = 0;
    info* a = head;
    while (a) {
        sum = sum + a->rent;
        sum = sum + a->parkingFee;
        sum = sum + a->waterBill;
        a = a->next;
    }
    return sum;
}
