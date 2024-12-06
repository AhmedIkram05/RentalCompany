// Vehicle.h
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>

class Vehicle {
protected:
    std::string vehicleID;
    std::string make;
    std::string model;
    int passengers;
    int capacity;
    bool availability;
    double lateFee; // Late fee per day

public:
    // Constructors
    Vehicle();
    Vehicle(const std::string& id, const std::string& mk, const std::string& mdl,
            int passengers, int storage, bool avail);

    // Destructor
    virtual ~Vehicle();

    // Getters
    std::string getVehicleID() const;
    std::string getMake() const;
    std::string getModel() const;
    int getPassengers() const;
    int getCapacity() const;
    bool getAvailability() const;
    double getLateFee() const;

    // Setters
    void setAvailability(bool avail);
    void setLateFee(double fee);

    // Virtual function to display vehicle details
    virtual void displayVehicle() const = 0;
};

#endif // VEHICLE_H
