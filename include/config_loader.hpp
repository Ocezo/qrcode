#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP

#include <string>

#include "url_params.hpp"

UrlParams load_url_params(const std::string& config_path);

#endif
