#include "config_loader.hpp"

#include <fstream>
#include <stdexcept>

namespace {

std::string trim(std::string value) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = value.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return {};
    }

    const std::size_t last = value.find_last_not_of(whitespace);
    return value.substr(first, last - first + 1);
}

std::string unquote(std::string value) {
    value = trim(value);
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
        return value.substr(1, value.size() - 2);
    }
    return value;
}

}  // namespace

UrlParams load_url_params(const std::string& config_path) {
    std::ifstream file(config_path);
    if (file.is_open() == false) {
        throw std::runtime_error("Failed to open config file: " + config_path);
    }

    UrlParams params;
    bool inside_url_params = false;
    std::string line;

    while (std::getline(file, line)) {
        const std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') {
            continue;
        }

        if (trimmed == "url_params:") {
            inside_url_params = true;
            continue;
        }

        if (inside_url_params == false) {
            continue;
        }

        if (line.rfind("  ", 0) == std::string::npos) {
            break;
        }

        const std::size_t separator = trimmed.find(':');
        if (separator == std::string::npos) {
            continue;
        }

        const std::string name = trim(trimmed.substr(0, separator));
        const std::string value = unquote(trimmed.substr(separator + 1));

        if (name == "website") {
            params.website = value;
        } else if (name == "page") {
            params.page = value;
        } else if (name == "key") {
            params.key = value;
        }
    }

    if (params.website.empty() || params.page.empty() || params.key.empty()) {
        throw std::runtime_error(
            "Config file must define non-empty 'website', 'page' and 'key' fields.");
    }

    return params;
}
