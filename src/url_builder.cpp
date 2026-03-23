#include "url_builder.hpp"

std::string build_target_url(const std::string& anchor, const UrlParams& params) {
    return params.website + "/" + params.page + "?" + params.key + "=" + params.key + anchor;
}

std::string sanitize_filename(const std::string& value) {
    std::string result;
    result.reserve(value.size());

    for (const char ch : value) {
        const bool is_alpha_num = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                                  (ch >= '0' && ch <= '9');
        result.push_back(is_alpha_num ? ch : '_');
    }

    return result.empty() ? "output" : result;
}
