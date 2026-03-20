#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <fcntl.h>
#include <sys/wait.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>

namespace {

struct UrlParams {
    std::string website;
    std::string page;
    std::string key;
};

std::string trim_trailing_newlines(std::string value) {
    while (value.empty() == false && (value.back() == '\n' || value.back() == '\r')) {
        value.pop_back();
    }
    return value;
}

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

std::string read_from_stdin() {
    std::ostringstream buffer;
    buffer << std::cin.rdbuf();
    return trim_trailing_newlines(buffer.str());
}

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <text>\n"
              << "   or: echo \"text\" | " << program_name << "\n";
}

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

std::string build_target_url(const std::string& anchor, const UrlParams& params) {
    return params.website + "/" + params.page + "/#" + params.key + anchor;
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

std::string make_temp_path() {
    std::string path = "/tmp/qrcode_XXXXXX.png";
    int fd = mkstemps(path.data(), 4);
    if (fd == -1) {
        return {};
    }

    close(fd);
    return path;
}

bool run_qrencode(const std::string& input, const std::string& output_path) {
    const pid_t child_pid = fork();

    if (child_pid == -1) {
        return false;
    }

    if (child_pid == 0) {
        execlp(
            "qrencode",
            "qrencode",
            "-o",
            output_path.c_str(),
            "--",
            input.c_str(),
            static_cast<char*>(nullptr));
        _exit(127);
    }

    int status = 0;
    if (waitpid(child_pid, &status, 0) == -1) {
        return false;
    }

    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

bool generate_qr_image(const std::string& input, const std::string& output_path) {
    const std::string temp_path = make_temp_path();
    if (temp_path.empty()) {
        std::cerr << "Error: failed to create a temporary file path.\n";
        return false;
    }

    if (run_qrencode(input, temp_path) == false) {
        std::remove(temp_path.c_str());
        std::cerr << "Error: failed to generate a QR code with qrencode.\n";
        return false;
    }

    const cv::Mat image = cv::imread(temp_path, cv::IMREAD_UNCHANGED);
    std::remove(temp_path.c_str());

    if (image.empty()) {
        std::cerr << "Error: failed to load the generated QR code image.\n";
        return false;
    }

    if (cv::imwrite(output_path, image) == false) {
        std::cerr << "Error: failed to write output image to " << output_path << '\n';
        return false;
    }

    return true;
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
        std::cerr << "Error: missing input text. Run the program with at least one argument, for example: ./qrcode bag_1\n";
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

    std::cout << "Input received: " << input << '\n';
    std::cout << "Encoded URL: " << target_url << '\n';

    const std::string output_file = std::string(PROJECT_SOURCE_DIR) + "/img/qrcode_" + sanitize_filename(input) + ".png";
    if (generate_qr_image(target_url, output_file) == false) {
        return 1;
    }

    std::cout << "Image written to: " << output_file << '\n';

    return 0;
}
