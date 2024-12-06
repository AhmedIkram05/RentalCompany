// RentalCompany.cpp
#include "RentalCompany.h"
#include "Car.h"
#include "Van.h"
#include "Minibus.h"
#include "SUV.h"
#include "Customer.h"
#include "DateUtils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <regex>

// Constructor
RentalCompany::RentalCompany() {}

// Destructorn
RentalCompany::~RentalCompany() {}

// Levenshtein  Distance Algorithm
int RentalCompany::levenshteinDistance(const std::string& s1, const std::string& s2) const {
    const size_t m = s1.size();
    const size_t n = s2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    // Initialize first column
    for (size_t i = 0; i <= m; ++i) {
        dp[i][0] = static_cast<int>(i); // Explicit cast to int
    }

    // Initialize first row
    for (size_t j = 0; j <= n; ++j) {
        dp[0][j] = static_cast<int>(j); // Explicit cast to int
    }

    // Compute Levenshtein distance
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = 1 + std::min({ dp[i - 1][j],     // Deletion
                                          dp[i][j - 1],     // Insertion
                                          dp[i - 1][j - 1]  // Substitution
                                        });
            }
        }
    }

    return dp[m][n];
}

// Add a new vehicle (with duplication check)
void RentalCompany::addVehicle(const std::shared_ptr<Vehicle>& vehicle) {
    if (searchVehicle(vehicle->getVehicleID()) != nullptr) {
        throw std::runtime_error("Error: Vehicle ID " + vehicle->getVehicleID() + " already exists.");
    }
    vehicles.push_back(vehicle);
}

// Remove a vehicle by its ID
void RentalCompany::removeVehicle(const std::string& vehicleID) {
    auto it = std::find_if(vehicles.begin(), vehicles.end(),
                           [&](const std::shared_ptr<Vehicle>& v) { return v->getVehicleID() == vehicleID; });
    if (it != vehicles.end()) {
        vehicles.erase(it);
        std::cout << "Vehicle " << vehicleID << " removed successfully.\n";
    }
    else {
        std::cerr << "Vehicle ID " << vehicleID << " not found.\n";
    }
}

// Display all available vehicles
void RentalCompany::displayAvailableVehicles() const {
    bool anyAvailable = false;
    for (const auto& vehicle : vehicles) {
        if (vehicle->getAvailability()) {
            vehicle->displayVehicle();
            anyAvailable = true;
        }
    }
    if (!anyAvailable) {
        std::cout << "No vehicles are currently available for rent.\n";
    }
}

// Display all vehicles
void RentalCompany::displayAllVehicles() const {
    if (vehicles.empty()) {
        std::cout << "No vehicles in the system.\n";
        return;
    }
    for (const auto& vehicle : vehicles) {
        vehicle->displayVehicle();
    }
}

// Search for a vehicle by its ID
std::shared_ptr<Vehicle> RentalCompany::searchVehicle(const std::string& vehicleID) const {
    auto it = std::find_if(vehicles.begin(), vehicles.end(),
                           [&](const std::shared_ptr<Vehicle>& v) { return v->getVehicleID() == vehicleID; });
    if (it != vehicles.end()) {
        return *it;
    }
    return nullptr;
}

// Add a new customer (with duplication check)
void RentalCompany::addCustomer(const Customer& customer) {
    if (searchCustomer(customer.getCustomerID()) != nullptr) {
        std::cerr << "Customer ID " << customer.getCustomerID() << " already exists." << std::endl;
        return;
    }
    customers.push_back(customer);
}

// Remove a customer by ID
void RentalCompany::removeCustomer(int customerID) {
    auto it = std::find_if(customers.begin(), customers.end(),
                           [&](const Customer& c) { return c.getCustomerID() == customerID; });
    if (it != customers.end()) {
        customers.erase(it);
        std::cout << "Customer ID " << customerID << " removed successfully.\n";
    } else {
        std::cerr << "Error: Customer ID " << customerID << " not found.\n";
    }
}

// Display all customers
void RentalCompany::displayCustomers() const {
    if (customers.empty()) {
        std::cout << "No customers in the system.\n";
        return;
    }
    std::cout << "=== All Customers ===\n";
    for (const auto& customer : customers) {
        customer.displayCustomer();
    }
}

// Search for a customer by ID
Customer* RentalCompany::searchCustomer(int customerID) {
    auto it = std::find_if(customers.begin(), customers.end(),
                           [&](Customer& c) { return c.getCustomerID() == customerID; });
    if (it != customers.end()) {
        return &(*it);
    }
    return nullptr;
}

// Rent a vehicle
void RentalCompany::rentVehicle(int customerID, const std::string& vehicleID) {
    Customer* customer = searchCustomer(customerID);
    if (!customer) {
        throw std::runtime_error("Customer ID " + std::to_string(customerID) + " not found.");
    }

    std::shared_ptr<Vehicle> vehicle = searchVehicle(vehicleID);
    if (!vehicle) {
        throw std::runtime_error("Vehicle ID " + vehicleID + " not found.");
    }

    if (!vehicle->getAvailability()) {
        throw std::runtime_error("Vehicle ID " + vehicleID + " is not available.");
    }

    // Set rental dates
    std::string rentDate = DateUtils::getCurrentDate();
    std::string dueDate = DateUtils::addDays(rentDate, 7); // 7 days rental

    try {
        vehicle->setAvailability(false);
        customer->rentVehicle(vehicle, rentDate, dueDate);
        std::cout << "Vehicle " << vehicleID << " rented to customer " << customer->getName() << " successfully.\n";
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error during rental: " + std::string(e.what()));
    }
}

// Return a vehicle
void RentalCompany::returnVehicle(int customerID, const std::string& vehicleID, const std::string& returnDate) {
    Customer* customer = searchCustomer(customerID);
    if (!customer) {
        throw std::runtime_error("Customer ID " + std::to_string(customerID) + " not found.");
    }

    std::shared_ptr<Vehicle> vehicle = searchVehicle(vehicleID);
    if (!vehicle) {
        throw std::runtime_error("Vehicle ID " + vehicleID + " not found.");
    }

    try {
        int daysLate = customer->returnVehicle(vehicle, returnDate);
        vehicle->setAvailability(true);
        if (daysLate > 0) {
            double totalLateFee = daysLate * vehicle->getLateFee();
            std::cout << "Vehicle " << vehicleID << " returned successfully. Days Late: " << daysLate
                      << ". Total Late Fee: $" << totalLateFee << "\n";
        }
        else {
            std::cout << "Vehicle " << vehicleID << " returned on time.\n";
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error returning vehicle: " + std::string(e.what()));
    }
}

// Load data from files
void RentalCompany::loadFromFile(const std::string& vehiclesFile, const std::string& customersFile) {
    // Load Vehicles
    std::ifstream vFile(vehiclesFile);
    if (!vFile.is_open()) {
        throw std::runtime_error("Unable to open vehicles file.");
    }

    std::string line;
    while (std::getline(vFile, line)) {
        std::istringstream iss(line);
        std::string type, id, make, model;
        int passengers, capacity, availInt;

        // Check if the first token is a recognized type
        iss >> type;
        if (type != "Car" && type != "Van" && type != "Minibus" && type != "SUV") {
            // If type is missing, assume a default type or handle accordingly
            // For example, default to Car and shift tokens
            iss.clear();
            iss.str(line);
            type = "Car"; // Default type
            if (!(iss >> id >> std::quoted(make) >> std::quoted(model) >> passengers >> capacity >> availInt)) {
                std::cerr << "Invalid vehicle entry: " << line << std::endl;
                continue;
            }
        } else {
            if (!(iss >> id >> std::quoted(make) >> std::quoted(model) >> passengers >> capacity >> availInt)) {
                std::cerr << "Invalid vehicle entry: " << line << std::endl;
                continue;
            }
        }

        bool available = (availInt == 1);
        std::shared_ptr<Vehicle> vehicle;

        if (type == "Car") {
            vehicle = std::make_shared<Car>(id, make, model, passengers, capacity, available);
        }
        else if (type == "Van") {
            vehicle = std::make_shared<Van>(id, make, model, passengers, capacity, available);
        }
        else if (type == "Minibus") {
            vehicle = std::make_shared<Minibus>(id, make, model, passengers, capacity, available);
        }
        else if (type == "SUV") {
            vehicle = std::make_shared<SUV>(id, make, model, passengers, capacity, available);
        }
        else {
            std::cerr << "Unknown vehicle type: " << type << std::endl;
            continue;
        }

        vehicles.push_back(vehicle);
        std::cout << "Vehicle " << id << " added successfully." << std::endl;
    }
    vFile.close();

    // Load Customers
    std::ifstream cFile(customersFile);
    if (!cFile.is_open()) {
        throw std::runtime_error("Unable to open customers file.");
    }

    while (std::getline(cFile, line)) {
        std::istringstream iss(line);
        int customerID;
        std::string name;
        iss >> customerID >> name;

        Customer customer(customerID, name);

        std::string vehicleID;
        while (iss >> vehicleID) {
            auto vehicle = searchVehicle(vehicleID);
            if (vehicle) {
                if (vehicle->getAvailability()) {
                    std::string rentDate = DateUtils::getCurrentDate();
                    std::string dueDate = DateUtils::addDays(rentDate, 7); // 7 days rental
                    customer.rentVehicle(vehicle, rentDate, dueDate);
                    vehicle->setAvailability(false);
                    std::cout << "Vehicle " << vehicleID << " rented to customer " << name << " successfully." << std::endl;
                }
                else {
                    std::cout << "Vehicle " << vehicleID << " is not available for rent." << std::endl;
                }
            }
            else {
                std::cout << "Vehicle " << vehicleID << " does not exist." << std::endl;
            }
        }

        addCustomer(customer);
        std::cout << "Customer " << name << " added successfully." << std::endl;
    }
    cFile.close();

    std::cout << "Data loaded successfully." << std::endl;
}

// Save data to files
void RentalCompany::saveToFile(const std::string& vehiclesFile, const std::string& customersFile) const {
    // Save Vehicles
    std::ofstream vFile(vehiclesFile);
    if (!vFile.is_open()) {
        throw std::runtime_error("Failed to open vehicles file for writing: " + vehiclesFile);
    }

    for (const auto& vehicle : vehicles) {
        std::string type;
        if (dynamic_cast<Car*>(vehicle.get())) {
            type = "Car";
        } else if (dynamic_cast<Van*>(vehicle.get())) {
            type = "Van";
        } else if (dynamic_cast<Minibus*>(vehicle.get())) {
            type = "Minibus";
        } else {
            type = "Unknown";
        }

        if (type == "Unknown") {
            continue; // Skip unknown vehicle types
        }

        vFile << type << " " << vehicle->getVehicleID() << " "
              << std::quoted(vehicle->getMake()) << " "
              << std::quoted(vehicle->getModel()) << " "
              << vehicle->getPassengers() << " "
              << vehicle->getCapacity() << " "
              << (vehicle->getAvailability() ? "1" : "0") << "\n";
    }
    vFile.close();

    // Save Customers
    std::ofstream cFile(customersFile);
    if (!cFile.is_open()) {
        throw std::runtime_error("Failed to open customers file for writing: " + customersFile);
    }

    for (const auto& customer : customers) {
        cFile << customer.getCustomerID() << " ";

        // Write name with quotes if it contains spaces
        if (customer.getName().find(' ') != std::string::npos) {
            cFile << std::quoted(customer.getName());
        } else {
            cFile << customer.getName();
        }

        for (const auto& rental : customer.getRentedVehicles()) {
            cFile << " " << rental.vehicle->getVehicleID();
        }
        cFile << "\n";
    }
    cFile.close();

    std::cout << "Data saved successfully to " << vehiclesFile << " and " << customersFile << ".\n";
}

// Search Vehicles with Fuzzy Matching
std::vector<std::shared_ptr<Vehicle>> RentalCompany::searchVehicles(const SearchCriteria& criteria) const {
    std::vector<std::shared_ptr<Vehicle>> results;

    for (const auto& vehicle : vehicles) {
        bool match = true;

        // Fuzzy match for make
        if (!criteria.make.empty()) {
            int distance = levenshteinDistance(vehicle->getMake(), criteria.make);
            if (distance > criteria.maxDistance) {
                match = false;
            }
        }

        // Fuzzy match for model
        if (match && !criteria.model.empty()) {
            int distance = levenshteinDistance(vehicle->getModel(), criteria.model);
            if (distance > criteria.maxDistance) {
                match = false;
            }
        }

        // Filter by passenger capacity
        if (match && criteria.passengerCapacity != -1) {
            if (vehicle->getPassengers() < criteria.passengerCapacity) {
                match = false;
            }
        }

        // Filter by storage capacity
        if (match && criteria.storageCapacity != -1) {
            if (vehicle->getCapacity() < criteria.storageCapacity) {
                match = false;
            }
        }

        // Filter by availability
        if (match && criteria.filterByAvailability) {
            if (vehicle->getAvailability() != criteria.availability) {
                match = false;
            }
        }

        // Add to results if all criteria match
        if (match) {
            results.push_back(vehicle);
        }
    }

    return results;
}

// Search Customers with Fuzzy Matching
std::vector<Customer> RentalCompany::searchCustomers(const CustomerSearchCriteria& criteria) const {
    std::vector<Customer> results;

    for (const auto& customer : customers) {
        bool match = true;

        // Exact match for Customer ID
        if (criteria.customerID != -1) {
            if (customer.getCustomerID() != criteria.customerID) {
                match = false;
            }
        }

        // Fuzzy match for Name
        if (match && !criteria.name.empty()) {
            int distance = levenshteinDistance(customer.getName(), criteria.name);
            if (distance > criteria.maxDistance) {
                match = false;
            }
        }

        if (match) {
            results.push_back(customer);
        }
    }

    return results;
}

// Clear all data
void RentalCompany::clearData() {
    vehicles.clear();
    customers.clear();
    std::cout << "All data has been cleared successfully.\n";
}
