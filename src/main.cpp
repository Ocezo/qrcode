#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <fcntl.h>
#include <sys/wait.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

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
    std::string path = "/tmp/qrcode_XXXXXX";
    int fd = mkstemp(path.data());
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

    if (!run_qrencode(input, temp_path)) {
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

    if (!cv::imwrite(output_path, image)) {
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
    } else if (!isatty(STDIN_FILENO) && !std::cin.fail() && !std::cin.eof()) {
        input = read_from_stdin();
    }

    if (input.empty()) {
        std::cerr << "Error: missing input text. Run the program with at least one argument, for example: ./qrcode bag_1\n";
        print_usage(argv[0]);
        return 1;
    }

    std::cout << "Input received: " << input << '\n';

    const std::string output_file = "qrcode_" + sanitize_filename(input) + ".png";
    if (!generate_qr_image(input, output_file)) {
        return 1;
    }

    std::cout << "Image written to: " << output_file << '\n';

    return 0;
}
