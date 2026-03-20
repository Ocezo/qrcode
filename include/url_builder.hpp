#ifndef URL_BUILDER_HPP
#define URL_BUILDER_HPP

#include <string>

#include "url_params.hpp"

std::string build_target_url(const std::string& anchor, const UrlParams& params);
std::string sanitize_filename(const std::string& value);

#endif
