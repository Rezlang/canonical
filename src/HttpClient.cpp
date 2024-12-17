#include "HttpClient.hpp"

#include <curl/curl.h>

#include <iostream>

static size_t write_callback(void* contents, size_t size, size_t nmemb,
                             void* userp) {
  size_t totalSize = size * nmemb;
  std::string* str = static_cast<std::string*>(userp);
  str->append((char*)contents, totalSize);
  return totalSize;
}

HttpClient::HttpClient() {
  // Initialize libcurl
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

HttpClient::~HttpClient() {
  // Cleanup global libcurl resources
  curl_global_cleanup();
}

std::string HttpClient::download(const std::string& url) {
  // HttpClient performs a blocking GET request to the given URL
  // and returns the response body as a string.
  CURL* curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Failed to initialize curl handle\n";
    return "";
  }

  std::string responseBuffer;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
              << std::endl;
    curl_easy_cleanup(curl);
    return "";
  }

  long response_code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  curl_easy_cleanup(curl);

  if (response_code != 200) {
    std::cerr << "HTTP request returned status: " << response_code << std::endl;
    return "";
  }

  return responseBuffer;
}
