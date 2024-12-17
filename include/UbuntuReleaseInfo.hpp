#ifndef UBUNTU_RELEASE_INFO_HPP
#define UBUNTU_RELEASE_INFO_HPP

#include <string>
#include <vector>

class UbuntuReleaseInfo {
 public:
  virtual ~UbuntuReleaseInfo() = default;

  virtual bool populateData() = 0;

  virtual std::vector<std::string> getSupportedReleases() const = 0;

  virtual std::string getCurrentLTS() const = 0;

  virtual std::string getDiskImageSha256(const std::string& release) const = 0;
};

#endif  // UBUNTU_RELEASE_INFO_HPP
