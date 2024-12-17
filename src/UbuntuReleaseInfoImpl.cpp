#include <iostream>
#include <nlohmann/json.hpp>

#include "HttpClient.hpp"
#include "UbuntuReleaseInfo.hpp"

using json = nlohmann::json;

class UbuntuReleaseInfoImpl : public UbuntuReleaseInfo {
 public:
  bool populateData() override {
    // Downloads data and transforms to various data structures
    try {
      json root = downloadJson();

      validProducts.clear();
      currentLTS.clear();
      currentLTSVersion = 0.0;

      if (!root.contains("products") || !root["products"].is_object()) {
        std::cerr << "no products" << std::endl;
        return false;
      }

      const auto& products = root["products"];
      processProducts(products);
      return true;
    } catch (const std::exception& ex) {
      std::cerr << "Error populating data: " << ex.what() << std::endl;
      return false;
    }
  }

  std::vector<std::string> getSupportedReleases() const override {
    // Returns list of release titles for supported releases created by
    // populateData()
    std::vector<std::string> supportedReleases;
    supportedReleases.reserve(validProducts.size());

    for (const auto& [key, productJson] : validProducts) {
      if (productJson["supported"]) {
        supportedReleases.emplace_back(productJson["release_title"]);
      }
    }
    return supportedReleases;
  }

  std::string getCurrentLTS() const override {
    // Returns string version of most recently supported LTS release
    return currentLTS;
  }

  std::string getDiskImageSha256(const std::string& release) const override {
    // Returns disk image sha256 of given release
    auto it = validProducts.find(release);
    if (it == validProducts.end()) {
      return "";
    }
    const auto versions = it->second["versions"];
    if (versions.size() == 0) {
      return "";
    }
    return (*versions.begin())["items"]["disk1.img"]["sha256"];
  }

 private:
  void checkLTS(json value) {
    // checks if release is LTS and if version is greater than previously found
    // LTS releases. Helper function for populateData()
    std::string title = static_cast<std::string>(value["release_title"]);
    int LTSLoc = title.size() - 3;
    int actualLoc = title.find("LTS");
    if (LTSLoc != actualLoc) {
      return;
    }
    std::string version = value["version"];
    float dVersion = std::stod(version);
    if (dVersion > currentLTSVersion) {
      currentLTSVersion = dVersion;
      currentLTS = version;
    }
    return;
  }

  json downloadJson() {
    // Downloads Ubuntu version json data, returns parsed json object.
    // Helper function for populateData()
    std::string url =
        "https://cloud-images.ubuntu.com/releases/streams/v1/"
        "com.ubuntu.cloud:released:download.json";
    std::string jsonData = httpClient.download(url);
    if (jsonData.empty()) {
      std::cout << "json data is empty" << std::endl;
      return {};
    }

    return json::parse(jsonData);
  }

  void processProducts(const json& products) {
    // Adds release to validProducts map if made for amd64 architecture. calls
    // checkLTS for each added release. Helper for populateData()
    for (auto& [key, value] : products.items()) {
      if (value["arch"] == "amd64") {
        validProducts[key] = value;
        checkLTS(value);
      }
    }
  }

  //  Stores a map where the key is the product name and the value is the
  //  product json information.
  std::unordered_map<std::string, json> validProducts =
      std::unordered_map<std::string, json>();

  // Current LTS string and double
  std::string currentLTS;
  double currentLTSVersion = 0.0;

  HttpClient httpClient;
};

UbuntuReleaseInfo* createUbuntuReleaseInfo() {
  // Creates UbuntuReleaseInfo object
  return new UbuntuReleaseInfoImpl();
}
