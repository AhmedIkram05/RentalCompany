// RentalCompany.h
#ifndef RENTALCOMPANY_H
#define RENTALCOMPANY_H

#include <vector>
#include <string>
#include <memory>
#include "Vehicle.h"
#include "Customer.h"

// Define SearchCriteria struct outside the RentalCompany class
struct SearchCriteria {
    std::string make;
    std::string model;
    int maxDistance;
    int passengerCapacity;
    int storageCapacity;
    bool filterByAvailability;
    bool availability;

    // Default Constructor
    SearchCriteria()
        : make(""), model(""), maxDistance(0), passengerCapacity(-1),
          storageCapacity(-1), filterByAvailability(false), availability(false) {}
};

// Define CustomerSearchCriteria struct outside the RentalCompany class
struct CustomerSearchCriteria {
    int customerID;
    std::string name;
    int maxDistance;

    // Default Constructor
    CustomerSearchCriteria()
        : customerID(-1), name(""), maxDistance(0) {}
};

class RentalCompany {
public:
    // Constructor
    RentalCompany();

    // Destructor
    ~RentalCompany();

    // Load data from files
    void loadFromFile(const std::string& vehiclesFile, const std::string& customersFile);

    // Save data to files
    void saveToFile(const std::string& vehiclesFile, const std::string& customersFile) const;

    // Add a new vehicle (with duplication check)
    void addVehicle(const std::shared_ptr<Vehicle>& vehicle);

    // Remove a vehicle by its ID
    void removeVehicle(const std::string& vehicleID);

    // Display all available vehicles
    void displayAvailableVehicles() const;

    // Display all vehicles
    void displayAllVehicles() const;

    // Search for a vehicle by its ID
    std::shared_ptr<Vehicle> searchVehicle(const std::string& vehicleID) const;

    // Add a new customer (with duplication check)
    void addCustomer(const Customer& customer);

    // Remove a customer by ID
    void removeCustomer(int customerID);

    // Search for a customer by ID
    Customer* searchCustomer(int customerID);

    // Display all customers
    void displayCustomers() const;

    // Levenshtein Distance
    int levenshteinDistance(const std::string& s1, const std::string& s2) const;

    // Clear all data
    void clearData();

    // Search Vehicles with Fuzzy Matching (Always Enabled)
    std::vector<std::shared_ptr<Vehicle>> searchVehicles(const SearchCriteria& criteria) const;

    // Search Customers with Fuzzy Matching
    std::vector<Customer> searchCustomers(const CustomerSearchCriteria& criteria) const;

    // Rent and Return Vehicles
    void rentVehicle(int customerID, const std::string& vehicleID);
    void returnVehicle(int customerID, const std::string& vehicleID, const std::string& returnDate);

private:
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::vector<Customer> customers;
};

#endif // RENTALCOMPANY_H
