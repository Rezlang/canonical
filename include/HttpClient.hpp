#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <string>

class HttpClient {
 public:
  HttpClient();
  ~HttpClient();

  std::string download(const std::string& url);
};

#endif  // HTTP_CLIENT_HPP
