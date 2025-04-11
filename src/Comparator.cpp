#include "../include/Comparator.h"
#include "../include/rpmvercmp.h"
#include <set>

void Comparator::CompareArchitectures(const std::string& branch_name_lhs, const Map& lhs,
                                      const std::string& branch_name_rhs, const Map& rhs,
                                      Poco::JSON::Object::Ptr result) {
    std::set<std::string> all_archs;
    for (const auto& [arch, _] : lhs) {
        all_archs.insert(arch);
    }
    for (const auto& [arch, _] : rhs) {
        all_archs.insert(arch);
    }

    for (const auto& arch : all_archs) {
        Poco::JSON::Object::Ptr arch_result = new Poco::JSON::Object;
        auto lhs_arch = lhs.find(arch);
        auto rhs_arch = rhs.find(arch);
        if (lhs_arch == lhs.end()) {
            AddAllPackages(arch_result, "only_in_" + branch_name_rhs, rhs_arch->second);
        } else if (rhs_arch == rhs.end()) {
            AddAllPackages(arch_result, "only_in_" + branch_name_lhs, lhs_arch->second);
        } else {
            CompareSingleArchitecture(branch_name_lhs, lhs_arch->second, branch_name_rhs,
                                      rhs_arch->second, arch_result);
        }
        result->set(arch, arch_result);
    }
}

void Comparator::AddAllPackages(Poco::JSON::Object::Ptr arch_result, const std::string& category,
                                const std::map<std::string, Package>& packages) {
    Poco::JSON::Array::Ptr arr = new Poco::JSON::Array;
    for (const auto& [name, pkg] : packages) {
        Poco::JSON::Object::Ptr obj = new Poco::JSON::Object;
        obj->set("name", name);
        obj->set("version", pkg.RpmVersion());
        arr->add(obj);
    }
    arch_result->set(category, arr);
}

void Comparator::CompareSingleArchitecture(const std::string& branch_name_lhs,
                                           const std::map<std::string, Package>& lhs,
                                           const std::string& branch_name_rhs,
                                           const std::map<std::string, Package>& rhs,
                                           Poco::JSON::Object::Ptr arch_result) {
    Poco::JSON::Array::Ptr only_lhs = new Poco::JSON::Array;
    Poco::JSON::Array::Ptr only_rhs = new Poco::JSON::Array;
    Poco::JSON::Array::Ptr newer = new Poco::JSON::Array;

    for (const auto& [name, pkg] : rhs) {
        if (!lhs.count(name)) {
            Poco::JSON::Object::Ptr obj = new Poco::JSON::Object;
            obj->set("name", name);
            obj->set("version", pkg.RpmVersion());
            only_rhs->add(obj);
        }
    }

    for (const auto& [name, pkg] : lhs) {
        if (!rhs.count(name)) {
            Poco::JSON::Object::Ptr obj = new Poco::JSON::Object;
            obj->set("name", name);
            obj->set("version", pkg.RpmVersion());
            only_lhs->add(obj);
        }
    }

    for (const auto& [name, b1_pkg] : lhs) {
        if (rhs.count(name)) {
            const auto& b2_pkg = rhs.at(name);
            int cmp = CompareVersions(b1_pkg, b2_pkg);

            if (cmp > 0) {
                Poco::JSON::Object::Ptr obj = new Poco::JSON::Object;
                obj->set("name", name);
                obj->set(branch_name_rhs, b2_pkg.RpmVersion());
                obj->set(branch_name_lhs, b1_pkg.RpmVersion());
                newer->add(obj);
            }
        }
    }

    arch_result->set("only_in_" + branch_name_lhs, only_lhs);
    arch_result->set("only_in_" + branch_name_rhs, only_rhs);
    arch_result->set("newer_in_" + branch_name_lhs, newer);
}

int Comparator::CompareVersions(const Package& lhs, const Package& rhs) {
    return Rpmvercmp(lhs.RpmVersion().c_str(), rhs.RpmVersion().c_str());
}

Poco::JSON::Object::Ptr Comparator::Compare(const std::string& branch_name_lhs, const Map& lhs,
                                            const std::string& branch_name_rhs, const Map& rhs) {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
    CompareArchitectures(branch_name_lhs, lhs, branch_name_rhs, rhs, result);
    return result;
}