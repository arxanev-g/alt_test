#pragma once

#include <memory>
#include <stdexcept>

#include "Types.h"

struct APIError : std::runtime_error {
    APIError(int http_code, std::string details)
        : std::runtime_error{"api error: code=" + std::to_string(http_code) +
                             " details=" + details},
          http_code{http_code},
          details{std::move(details)} {
    }

    int http_code;
    std::string details;
};

class BaseLoader {
public:
    virtual ~BaseLoader() = default;
    virtual Map Load() = 0;
};

std::unique_ptr<BaseLoader> CreateLoader(
    const std::string& branch,
    const std::string& endpoint = "https://rdb.altlinux.org/api/export/branch_binary_packages/");