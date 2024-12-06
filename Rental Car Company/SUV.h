// SUV.h
#ifndef SUV_H
#define SUV_H

#include "Vehicle.h"

class SUV : public Vehicle {
public:
    // Constructors
    SUV(const std::string& id, const std::string& mk, const std::string& mdl,
        int passengers, int storage, bool avail);

    // Destructor
    ~SUV();

    // Override displayVehicle
    void displayVehicle() const override;
};

#endif // SUV_H