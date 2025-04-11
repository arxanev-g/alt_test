#pragma once
#include "Types.h"
#include <Poco/JSON/Object.h>

class Comparator {
public:
    Poco::JSON::Object::Ptr Compare(const std::string& branch_name_lhs,const Map& lhs,const std::string& branch_name_rhs, const Map& rhs);

private:
    void CompareArchitectures(const std::string& branch_name_lhs ,const Map& lhs,const std::string& branch_name_rhs, const Map& rhs,
                                             Poco::JSON::Object::Ptr result);
    void CompareSingleArchitecture(
    const std::string& branch_name_lhs,
    const std::map<std::string, Package>& lhs,
    const std::string& branch_name_rhs,
    const std::map<std::string, Package>& rhs, Poco::JSON::Object::Ptr arch_result);
    static int CompareVersions(const Package& lhs, const Package& rhs);
    void AddAllPackages(
    Poco::JSON::Object::Ptr arch_result,
    const std::string& category,
    const std::map<std::string, Package>& packages
);
};