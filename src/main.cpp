#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "config_loader.hpp"
#include "qr_generator.hpp"
#include "url_builder.hpp"

namespace {

std::string trim_trailing_newlines(std::string value) {
    while (value.empty() == false && (value.back() == '\n' || value.back() == '\r')) {
        value.pop_back();
    }
    return value;
}

std::string read_from_stdin() {
    std::ostringstream buffer;
    buffer << std::cin.rdbuf();
    return trim_trailing_newlines(buffer.str());
}

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <text>\n"
              << "   or: echo \"text\" | " << program_name << "\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc > 1) {
        const std::string first_arg = argv[1];
        if (first_arg == "--help" || first_arg == "-h") {
            print_usage(argv[0]);
            return 0;
        }
    }

    std::string input;

    if (argc > 1) {
        input = argv[1];
    } else if (isatty(STDIN_FILENO) == 0 && std::cin.fail() == false && std::cin.eof() == false) {
        input = read_from_stdin();
    }

    if (input.empty()) {
        std::cerr << "Error: missing input text. Run the program with at least one argument, for example: ./qrcode 1\n";
        print_usage(argv[0]);
        return 1;
    }

    const std::string config_path = std::string(PROJECT_SOURCE_DIR) + "/config/params.yaml";

    UrlParams params;
    try {
        params = load_url_params(config_path);
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    const std::string target_url = build_target_url(input, params);

    std::cout << "Input received  : " << input << '\n';
    std::cout << "Encoded URL     : " << target_url << '\n';

    const std::string output_file = std::string(PROJECT_SOURCE_DIR) + "/img/qrcode_bag" + sanitize_filename(input) + ".png";
    if (generate_qr_image(target_url, output_file) == false) {
        return 1;
    }

    std::cout << "Image written to: " << output_file << '\n';

    return 0;
}
