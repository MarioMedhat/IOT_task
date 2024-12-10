#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "JsonDataRW.hpp"

static bool isFileEmpty(std::ifstream& file) {
    return file.peek() == std::ifstream::traits_type::eof();
}

void addJsonNewData(const std::string& filePath, nlohmann::json newJsonData) {
    // Read existing JSON data
    nlohmann::json jsonData;

    std::ifstream inFile(filePath);
    if (inFile.is_open()) {
        if (isFileEmpty(inFile)) {
            // Initialize an empty JSON array if the file is empty
            jsonData = nlohmann::json::array();
        } else {
            // Parse the JSON data from the file
            inFile >> jsonData;
        }
        inFile.close();
    } else {
        // Initialize an empty JSON array if the file does not exist
        jsonData = nlohmann::json::array();
    }

    // Add the new reading to the array
    jsonData.push_back(newJsonData);

    // Write back to the file
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << jsonData.dump(4); // Pretty-print with an indentation of 4 spaces
        outFile.close();
    } else {
        std::cerr << "Unable to open file for writing.\n";
    }
}

void writeJsonFile(const std::string& filepath) {
    nlohmann::json jsonData;
    // Simulate data
    jsonData["message"] = "Hello, Subscriber!";
    jsonData["timestamp"] = 5555555;
    jsonData["status"] = "active";

    // Write JSON to file
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << jsonData.dump(4); // Pretty print with 4 spaces
        file.close();
    } else {
        std::cerr << "Failed to write to " << filepath << std::endl;
    }
}

void updateJsonFile(const std::string& filepath, std::string field, int val)
{
    // Create or load the JSON file
    std::ifstream file(filepath);
    nlohmann::json jsonData;

    if (file.is_open() && file.peek() != std::ifstream::traits_type::eof()) {
        // If file exists and is not empty, load the content
        file >> jsonData;

        jsonData[field] = val;
    }
    file.close();
    std::ofstream outfile(filepath);
    outfile << jsonData.dump();
    outfile.close();
}

void readJsonFile(const std::string& filepath, std::shared_ptr <nlohmann::json> jsonDataPtr) {
    if (!jsonDataPtr) {  // Check if jsonDataPtr is valid
        std::cerr << "Invalid shared pointer provided." << std::endl;
        return;
    }

    std::ifstream file(filepath);
    if (file.is_open()) {
        try {
            file >> *jsonDataPtr;  // Directly populate the JSON data into the shared pointer
            file.close();
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Failed to open " << filepath << std::endl;
    }
}

void resetJsonFile (const std::string& filepath) {

    nlohmann::json jsonData;

    // Write JSON to file
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << jsonData.dump(); // Pretty print with 4 spaces
        file.close();
    } else {
        std::cerr << "Failed to write to " << filepath << std::endl;
    }

}

std::string JsonDataToString (const std::string filepath)
{
    nlohmann::json jsonData;
    // Read JSON from file
    std::ifstream file(filepath);
    if (file.is_open()) {
            file >> jsonData;
            file.close();
    }
    return jsonData.dump();
}