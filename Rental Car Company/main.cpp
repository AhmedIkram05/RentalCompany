// main.cpp
#include "RentalCompany.h"
#include "Car.h"
#include "Van.h"
#include "Minibus.h"
#include "SUV.h"
#include "Customer.h"
#include "DateUtils.h"
#include <iostream>
#include <limits>
#include <string>

// Function declarations
void displayMainMenu();
void runSpecificTests(RentalCompany& company);
void handleRentVehicle(RentalCompany& company);
void handleReturnVehicle(RentalCompany& company);
void handleDisplayAvailableVehicles(RentalCompany& company);
void handleDisplayCustomers(RentalCompany& company);
void handleSearchVehicles(RentalCompany& company);
void handleSearchCustomers(RentalCompany& company);
void displaySearchResults(const std::vector<std::shared_ptr<Vehicle>>& results);
void displayCustomerSearchResults(const std::vector<Customer>& results);
void handleAddCustomer(RentalCompany& company);
void handleAddVehicle(RentalCompany& company);
void handleDisplayAllVehicles(RentalCompany& company);

int main() {
    RentalCompany company;

    // Load initial data from files
    try {
        company.loadFromFile("mainVehicles.txt", "mainCustomers.txt");
        std::cout << "Data loaded successfully.\n";
    } catch (const std::exception& e) {
        std::cout << "Error loading data: " << e.what() << "\n";
    }

    int choice;

    do {
        displayMainMenu();
        std::cin >> choice;

        // Validate input
        while (std::cin.fail() || choice < 1 || choice > 11) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 1 and 11: ";
            std::cin >> choice;
        }

        switch (choice) {
            case 1:
                handleAddCustomer(company);
                break;
            case 2:
                handleAddVehicle(company);
                break;
            case 3:
                handleRentVehicle(company);
                break;
            case 4:
                handleReturnVehicle(company);
                break;
            case 5:
                handleDisplayAvailableVehicles(company);
                break;
            case 6:
                handleDisplayAllVehicles(company);
                break;
            case 7:
                handleDisplayCustomers(company);
                break;
            case 8:
                handleSearchVehicles(company);
                break;
            case 9:
                handleSearchCustomers(company);
                break;
            case 10:
                runSpecificTests(company);
                break;
            case 11:
                std::cout << "Exiting program. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 11);

    return 0;
}

// Function definitions

void displayMainMenu() {
    std::cout << "\n=== Rental Company Menu ===\n";
    std::cout << "1. Add Customer\n";
    std::cout << "2. Add Vehicle\n";
    std::cout << "3. Rent Vehicle\n";
    std::cout << "4. Return Vehicle\n";
    std::cout << "5. Display Available Vehicles\n";
    std::cout << "6. Display All Vehicles\n";
    std::cout << "7. Display Customers\n";
    std::cout << "8. Search Vehicles\n";
    std::cout << "9. Search Customers\n";
    std::cout << "10. Run Specific Tests\n";
    std::cout << "11. Exit\n";
    std::cout << "Enter your choice: ";
}

// Run Specific Tests
void runSpecificTests(RentalCompany& company) {
    std::cout << "\n=== Running Specific Test Scenarios ===\n\n";

    // Clear existing data
    company.clearData();

    // Test 1: Loading sample data files...
    std::cout << "Test 1: Loading sample data files...\n";
    try {
        company.loadFromFile("vehicles.txt", "customers.txt");
        std::cout << "Test 1 PASSED: Sample data files loaded successfully.\n\n";
    } catch (const std::exception& e) {
        std::cout << "Test 1 FAILED: " << e.what() << "\n\n";
    }

    // Test 2: Adding new Car - V108 Vauxhall Corsa...
    std::cout << "Test 2: Adding new Car - V108 Vauxhall Corsa...\n";
    std::shared_ptr<Vehicle> car = std::make_shared<Car>("V108", "Vauxhall", "Corsa", 4, 32, true);
    company.addVehicle(car);
    std::cout << "Test 2 PASSED: Car V108 Vauxhall Corsa added successfully.\n\n";

    // Test 3: Adding new Customer - Christina (ID:106)...
    std::cout << "Test 3: Adding new Customer - Christina (ID:106)...\n";
    Customer customer(106, "Christina");
    company.addCustomer(customer);
    std::cout << "Test 3 PASSED: Customer Christina added successfully.\n\n";

    // Test 4a: David (ID:104) returning vehicle V106...
    std::cout << "Test 4a: David (ID:104) returning vehicle V106...\n";
    try {
        company.returnVehicle(104, "V106", DateUtils::getCurrentDate());
        // If no exception is thrown, the test should fail because we expect an error
        std::cout << "Test 4a FAILED: Expected error for non-existent customer, but operation succeeded.\n\n";
    }
    catch (const std::runtime_error& e) {
        // If the correct exception is caught, mark the test as passed
        std::cout << "Test 4a PASSED: Correctly handled error - " << e.what() << "\n\n";
    }
    catch (...) {
        // Catch any other unexpected exceptions
        std::cout << "Test 4a FAILED: Caught an unexpected type of exception.\n\n";
    }

    // Test 4b: Alice (ID:101) renting vehicle V106...
    std::cout << "Test 4b: Alice (ID:101) renting vehicle V106...\n";
    try {
        company.rentVehicle(101, "V106");
        std::cout << "Test 4b PASSED: Alice rented vehicle V106 successfully.\n\n";
    } catch (const std::exception& e) {
        std::cout << "Test 4b FAILED: " << e.what() << "\n\n";
    }

    // Test 5: Alice (ID:101) returning vehicle V106...
    std::cout << "Test 5: Alice (ID:101) returning vehicle V106...\n";
    try {
        company.returnVehicle(101, "V106", DateUtils::getCurrentDate());
        std::cout << "Test 5 PASSED: Alice returned vehicle V106 successfully.\n\n";
    } catch (const std::exception& e) {
        std::cout << "Test 5 FAILED: " << e.what() << "\n\n";
    }

    // Test 6: Searching for Car Make: Audi, Model: Q8...
    std::cout << "Test 6: Searching for Car Make: Audi, Model: Q8...\n";
    SearchCriteria criteria;
    criteria.make = "Audi";
    criteria.model = "Q8";
    std::vector<std::shared_ptr<Vehicle>> searchResults = company.searchVehicles(criteria);
    if (!searchResults.empty()) {
        std::cout << "Test 6 PASSED: Audi Q8 found.\n\n";
    } else {
        std::cout << "Test 6 FAILED: Audi Q8 not found.\n\n";
    }

    // Test 7a: Adding new Car - V109 Toyota Corolla...
    std::cout << "Test 7a: Adding new Car - V109 Toyota Corolla...\n";
    std::shared_ptr<Vehicle> car2 = std::make_shared<Car>("V109", "Toyota", "Corolla", 5, 35, true);
    company.addVehicle(car2);
    std::cout << "Test 7a PASSED: Car V109 Toyota Corolla added successfully.\n\n";

    // Test 7b: Bob (ID:102) renting vehicle V109...
    std::cout << "Test 7b: Bob (ID:102) renting vehicle V109...\n";
    try {
        company.rentVehicle(102, "V109");
        std::cout << "Test 7b PASSED: Bob rented vehicle V109 successfully.\n\n";
    } catch (const std::exception& e) {
        std::cout << "Test 7b FAILED: " << e.what() << "\n\n";
    }

    // Test 8: Exporting data to new files...
    std::cout << "Test 8: Exporting data to new files...\n";
    try {
        company.saveToFile("vehiclesTestOutput.txt", "customersTestOutput.txt");
        std::cout << "Test 8 PASSED: Data exported successfully to vehiclesTestOutput.txt and customersTestOutput.txt!\n\n";
    } catch (const std::exception& e) {
        std::cout << "Test 8 FAILED: " << e.what() << "\n\n";
    }

    std::cout << "=== Specific Test Scenarios Completed ===\n\n";
}

void handleRentVehicle(RentalCompany& company) {
    int customerID;
    std::string vehicleID;

    std::cout << "Enter Customer ID: ";
    std::cin >> customerID;

    std::cout << "Enter Vehicle ID to rent: ";
    std::cin >> vehicleID;

    try {
        company.rentVehicle(customerID, vehicleID);
        std::cout << "Vehicle rented successfully.\n\n";
    }
    catch (const std::exception& e) {
        std::cout << "Rental Failed: " << e.what() << "\n\n";
    }
}

void handleReturnVehicle(RentalCompany& company) {
    int customerID;
    std::string vehicleID;
    std::string returnDate;

    std::cout << "Enter Customer ID: ";
    std::cin >> customerID;

    std::cout << "Enter Vehicle ID to return: ";
    std::cin >> vehicleID;

    std::cout << "Enter Return Date (YYYY-MM-DD): ";
    std::cin >> returnDate;

    try {
        company.returnVehicle(customerID, vehicleID, returnDate);
        std::cout << "Vehicle returned successfully.\n\n";
    }
    catch (const std::exception& e) {
        std::cout << "Return Failed: " << e.what() << "\n\n";
    }
}

void handleDisplayAvailableVehicles(RentalCompany& company) {
    std::cout << "=== Available Vehicles ===\n";
    company.displayAvailableVehicles();
}

void handleDisplayAllVehicles(RentalCompany& company) {
    std::cout << "=== All Vehicles ===\n";
    company.displayAllVehicles();
}

void handleDisplayCustomers(RentalCompany& company) {
    company.displayCustomers();
}

void handleSearchVehicles(RentalCompany& company) {
    SearchCriteria criteria;
    int choice;
    bool addingCriteria = true;

    while (addingCriteria) {
        std::cout << "\nSelect a search criterion:\n";
        std::cout << "1. Make\n2. Model\n3. Passenger Capacity\n4. Storage Capacity\n5. Availability\n6. View Results\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string make;
                std::cout << "Enter make: ";
                std::cin.ignore();
                std::getline(std::cin, make);
                criteria.make = make;
                break;
            }
            case 2: {
                std::string model;
                std::cout << "Enter model: ";
                std::cin.ignore();
                std::getline(std::cin, model);
                criteria.model = model;
                break;
            }
            case 3: {
                int passengers;
                std::cout << "Enter minimum passenger capacity: ";
                std::cin >> passengers;
                criteria.passengerCapacity = passengers;
                break;
            }
            case 4: {
                int storage;
                std::cout << "Enter minimum storage capacity: ";
                std::cin >> storage;
                criteria.storageCapacity = storage;
                break;
            }
            case 5: {
                criteria.filterByAvailability = true;
                criteria.availability = true;
                break;
            }
            case 6:
                addingCriteria = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

        if (choice >=1 && choice <=5) {
            auto results = company.searchVehicles(criteria);
            std::cout << "\n" << results.size() << " matching vehicles found.\n";
            std::cout << "1. Continue Filtering\n2. View Results\nEnter choice: ";
            int subChoice;
            std::cin >> subChoice;
            if (subChoice == 2) {
                addingCriteria = false;
            }
        }
    }

    auto finalResults = company.searchVehicles(criteria);
    displaySearchResults(finalResults);

    std::cout << "\n1. Search Again\n2. Return to Main Menu\nEnter choice: ";
    int finalChoice;
    std::cin >> finalChoice;
    if (finalChoice == 1) {
        handleSearchVehicles(company);
    }
}

void handleSearchCustomers(RentalCompany& company) {
    CustomerSearchCriteria criteria;
    int subChoice;
    bool addingCriteria = true;

    while (addingCriteria) {
        std::cout << "\nSelect a search criterion for Customers:\n";
        std::cout << "1. Customer ID\n2. Name\n3. View Results\n";
        std::cout << "Enter choice: ";
        std::cin >> subChoice;

        switch (subChoice) {
            case 1: {
                std::cout << "Enter Customer ID: ";
                std::cin >> criteria.customerID;
                break;
            }
            case 2: {
                std::cout << "Enter Customer Name: ";
                std::cin.ignore();
                std::getline(std::cin, criteria.name);
                break;
            }
            case 3:
                addingCriteria = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

        if (subChoice == 1 || subChoice == 2) {
            auto results = company.searchCustomers(criteria);
            std::cout << "\n" << results.size() << " matching customers found.\n";
            std::cout << "1. Continue Filtering\n2. View Results\nEnter choice: ";
            int decision;
            std::cin >> decision;
            if (decision == 2) {
                addingCriteria = false;
            }
        }
    }

    auto finalResults = company.searchCustomers(criteria);
    displayCustomerSearchResults(finalResults);

    std::cout << "\n1. Search Again\n2. Return to Main Menu\nEnter choice: ";
    int finalChoice;
    std::cin >> finalChoice;
    if (finalChoice == 1) {
        handleSearchCustomers(company);
    }
}

void displaySearchResults(const std::vector<std::shared_ptr<Vehicle>>& results) {
    if (results.empty()) {
        std::cout << "No vehicles found matching the search criteria.\n";
        return;
    }

    std::cout << "\n=== Search Results ===\n";
    for (const auto& vehicle : results) {
        vehicle->displayVehicle();
    }
    std::cout << "=======================\n";
}

void displayCustomerSearchResults(const std::vector<Customer>& results) {
    if (results.empty()) {
        std::cout << "No customers match the search criteria.\n";
        return;
    }

    std::cout << "\n=== Search Results ===\n";
    for (const auto& customer : results) {
        customer.displayCustomer();
    }
}

void handleAddCustomer(RentalCompany& company) {
    int customerID;
    std::string name;

    std::cout << "Enter Customer ID: ";
    std::cin >> customerID;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    std::cout << "Enter Customer Name: ";
    std::getline(std::cin, name);

    try {
        company.addCustomer(Customer(customerID, name));
        std::cout << "Customer added successfully.\n\n";
    }
    catch (const std::exception& e) {
        std::cout << "Failed to add customer: " << e.what() << "\n\n";
    }
}

void handleAddVehicle(RentalCompany& company) {
    int vehicleType;
    std::string vehicleID, make, model;
    int passengers, storage;
    bool available;

    std::cout << "Select Vehicle Type:\n";
    std::cout << "1. Car\n";
    std::cout << "2. Van\n";
    std::cout << "3. Minibus\n";
    std::cout << "Enter choice: ";
    std::cin >> vehicleType;

    std::cout << "Enter Vehicle ID: ";
    std::cin >> vehicleID;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    std::cout << "Enter Make: ";
    std::getline(std::cin, make);

    std::cout << "Enter Model: ";
    std::getline(std::cin, model);

    std::cout << "Enter Passenger Capacity: ";
    std::cin >> passengers;

    std::cout << "Enter Storage Capacity: ";
    std::cin >> storage;

    std::cout << "Is the vehicle available? (1 for Yes, 0 for No): ";
    std::cin >> available;

    switch (vehicleType) {
        case 1: { // Car
            try {
                // Removed lateFeeRate parameter as it's fixed within the Car class
                company.addVehicle(std::make_shared<Car>(vehicleID, make, model, passengers, storage, available));
                std::cout << "Car added successfully.\n\n";
            }
            catch (const std::exception& e) {
                std::cout << "Failed to add Car: " << e.what() << "\n\n";
            }
            break;
        }
        case 2: { // Van
            try {
                // Van's lateFeeRate is fixed internally
                company.addVehicle(std::make_shared<Van>(vehicleID, make, model, passengers, storage, available));
                std::cout << "Van added successfully.\n\n";
            }
            catch (const std::exception& e) {
                std::cout << "Failed to add Van: " << e.what() << "\n\n";
            }
            break;
        }
        case 3: { // Minibus
            try {
                // Minibus's lateFeeRate is fixed internally
                company.addVehicle(std::make_shared<Minibus>(vehicleID, make, model, passengers, storage, available));
                std::cout << "Minibus added successfully.\n\n";
            }
            catch (const std::exception& e) {
                std::cout << "Failed to add Minibus: " << e.what() << "\n\n";
            }
            break;
        }
        default:
            std::cout << "Invalid vehicle type selected.\n\n";
    }
}
