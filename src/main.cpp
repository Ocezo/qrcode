#include <iostream>
#include <sstream>
#include <string>

namespace {

std::string trim_trailing_newlines(std::string value) {
    while (!value.empty() && (value.back() == '\n' || value.back() == '\r')) {
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
    } else if (!std::cin.fail() && !std::cin.eof()) {
        input = read_from_stdin();
    }

    if (input.empty()) {
        std::cerr << "Error: missing input text. Run the program with at least one argument, for example: ./qrcode bag_1\n";
        print_usage(argv[0]);
        return 1;
    }

    std::cout << "Input received: " << input << '\n';
    std::cout << "QR code generation is not implemented yet.\n";

    return 0;
}
