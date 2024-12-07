// Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <memory>
#include <iostream>

template <typename T>
void displayItems(const std::vector<std::shared_ptr<T>>& items) {
    for (const auto& item : items) {
        item->displayVehicle(); // Use displayVehicle for Vehicle
    }
}

// Specialization for Customer
template <>
void displayItems(const std::vector<std::shared_ptr<Customer>>& items) {
    for (const auto& item : items) {
        item->displayCustomer(); // Use displayCustomer for Customer
    }
}

#endif // UTILS_H
