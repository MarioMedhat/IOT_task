#ifndef JSON_DATA_RW_H
#define JSON_DATA_RW_H

#include <string>
#include <memory>
#include <nlohmann/json.hpp>

void addJsonNewData(const std::string& filePath, nlohmann::json newJsonData);
void writeJsonFile(const std::string& filepath);
void resetJsonFile (const std::string& filepath);
void readJsonFile(const std::string& filepath, std::shared_ptr <nlohmann::json> jsonDataPtr);
std::string JsonDataToString (const std::string filepath);
void updateJsonFile(const std::string& filepath, std::string field, int val);

#endif