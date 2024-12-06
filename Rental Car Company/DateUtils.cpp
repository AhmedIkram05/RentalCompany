// DateUtils.cpp
#include "DateUtils.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>

// Adds days to a given date string in YYYY-MM-DD format
std::string DateUtils::addDays(const std::string& dateStr, int daysToAdd) {
    std::tm tm = DateUtils::stringToTm(dateStr);

    // Convert to time_t and add days
    tm.tm_mday += daysToAdd;
    time_t time = std::mktime(&tm);
    if (time == -1) {
        throw std::runtime_error("Failed to convert and add days to date: " + dateStr);
    }

    // Convert back to string
    std::tm* newTm = std::localtime(&time);
    if (!newTm) {
        throw std::runtime_error("Failed to retrieve localtime.");
    }
    std::ostringstream oss;
    oss << std::put_time(newTm, "%Y-%m-%d");
    return oss.str();
}

// Validates if a date string is in YYYY-MM-DD format and is a valid date
bool DateUtils::isValidDate(const std::string& dateStr) {
    std::tm tm = DateUtils::stringToTm(dateStr);
    // Basic validation: Check year, month, day ranges
    if (tm.tm_year < 0 || tm.tm_mon < 0 || tm.tm_mon > 11 || tm.tm_mday <= 0 || tm.tm_mday > 31) {
        return false;
    }
    return true;
}

// Gets the current date in YYYY-MM-DD format
std::string DateUtils::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    if (!now_tm) {
        throw std::runtime_error("Failed to retrieve current local time.");
    }
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d");
    return oss.str();
}

// Calculates the difference in days between two dates in YYYY-MM-DD format
int DateUtils::daysDifference(const std::string& dueDateStr, const std::string& returnDateStr) {
    std::tm dueTm = stringToTm(dueDateStr);
    std::tm returnTm = stringToTm(returnDateStr);

    std::time_t dueTime = std::mktime(&dueTm);
    std::time_t returnTime = std::mktime(&returnTm);

    if (dueTime == -1 || returnTime == -1) {
        throw std::runtime_error("Invalid dates provided.");
    }

    double difference = std::difftime(returnTime, dueTime) / (60 * 60 * 24);
    return static_cast<int>(std::floor(difference));
}

// Converts a date string in YYYY-MM-DD format to std::tm
std::tm DateUtils::stringToTm(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse date: " + dateStr);
    }
    return tm;
}
