// Customer.cpp
#include "Customer.h"
#include "DateUtils.h"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Constructor
Customer::Customer(int id, const std::string& nm)
    : customerID(id), name(nm) {}

// Getters
int Customer::getCustomerID() const { return customerID; }
std::string Customer::getName() const { return name; }
std::vector<RentalInfo> Customer::getRentedVehicles() const { return rentedVehicles; }

// Rent a vehicle
void Customer::rentVehicle(const std::shared_ptr<Vehicle>& vehicle, const std::string& rentDate, const std::string& dueDate) {
    // Check if the vehicle is already rented
    if (hasRentedVehicle(vehicle)) {
        throw std::runtime_error("Error: Vehicle ID " + vehicle->getVehicleID() + " is already rented by this customer.");
    }
    // Create RentalInfo and add to rentedVehicles
    RentalInfo info;
    info.vehicle = vehicle;
    info.rentDate = rentDate;
    info.dueDate = dueDate;
    rentedVehicles.push_back(info);
}

// Return a vehicle and return daysLate
int Customer::returnVehicle(const std::shared_ptr<Vehicle>& vehicle, const std::string& returnDate) {
    auto it = std::find_if(rentedVehicles.begin(), rentedVehicles.end(),
                           [&](const RentalInfo& info) { return info.vehicle->getVehicleID() == vehicle->getVehicleID(); });
    if (it != rentedVehicles.end()) {
        // Calculate days late using DateUtils
        int daysLate = DateUtils::daysDifference(it->dueDate, returnDate);
        // Remove the rental info
        rentedVehicles.erase(it);
        return daysLate;
    } else {
        throw std::runtime_error("Error: Vehicle ID " + vehicle->getVehicleID() + " not found in rented vehicles.");
    }
}

// Display customer information
void Customer::displayCustomer() const {
    std::cout << "Customer ID: " << customerID << ", Name: " << name << "\n";
    if (rentedVehicles.empty()) {
        std::cout << "No rented vehicles.\n";
    } else {
        std::cout << "Rented Vehicles:\n";
        for (const auto& info : rentedVehicles) {
            std::cout << "  Vehicle ID: " << info.vehicle->getVehicleID()
                      << ", Rent Date: " << info.rentDate
                      << ", Due Date: " << info.dueDate << "\n";
        }
    }
}

// Check if customer has rented a specific vehicle
bool Customer::hasRentedVehicle(const std::shared_ptr<Vehicle>& vehicle) const {
    return std::any_of(rentedVehicles.begin(), rentedVehicles.end(),
                       [&](const RentalInfo& info) { return info.vehicle->getVehicleID() == vehicle->getVehicleID(); });
}
