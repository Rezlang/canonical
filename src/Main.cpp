#include <iostream>
#include <memory>

#include "UbuntuReleaseInfo.hpp"

// Declare the factory function that creates the concrete instance
UbuntuReleaseInfo* createUbuntuReleaseInfo();

static void printUsage(const char* progName) {
  std::cout
      << "Usage:\n"
      << "  " << progName
      << " --list            List all currently supported Ubuntu releases\n"
      << "  " << progName
      << " --lts             Show the current Ubuntu LTS version\n"
      << "  " << progName
      << " --sha256 RELEASE  Show the sha256 for disk1.img of the given "
         "RELEASE (e.g. com.ubuntu.cloud:server:20.04:amd64)\n"
      << "Example:\n"
      << "  " << progName << " --sha256 com.ubuntu.cloud:server:20.04:amd64\n"
      << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printUsage(argv[0]);
    return 1;
  }

  std::unique_ptr<UbuntuReleaseInfo> info(createUbuntuReleaseInfo());

  if (!info->populateData()) {
    std::cerr << "Failed to fetch or parse Ubuntu release data.\n";
    return 1;
  }

  std::string arg1 = argv[1];
  if (arg1 == "--list") {
    auto releases = info->getSupportedReleases();
    for (auto& rel : releases) {
      std::cout << rel << std::endl;
    }
  } else if (arg1 == "--lts") {
    std::string lts = info->getCurrentLTS();
    if (lts.empty()) {
      std::cout << "No current LTS found.\n";
    } else {
      std::cout << "Current LTS: " << lts << std::endl;
    }
  } else if (arg1 == "--sha256") {
    if (argc < 3) {
      std::cerr << "Please specify a release name (e.g. 22.04)\n";
      return 1;
    }
    std::string releaseArg = argv[2];
    std::string sha = info->getDiskImageSha256(releaseArg);
    if (sha.empty()) {
      std::cerr << "No disk1.img sha256 found for release " << releaseArg
                << std::endl;
      return 1;
    }
    std::cout << "sha256(" << releaseArg << "): " << sha << std::endl;
  } else {
    printUsage(argv[0]);
    return 1;
  }

  return 0;
}
