#pragma once

#include <string>
#include <map>

struct Package {
    std::string branch;
    std::string version;
    std::string release;

    std::string RpmVersion() const {
        return version + "-" + release;
    }
};

using Map = std::map<std::string, std::map<std::string, Package>>;