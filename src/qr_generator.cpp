#include "qr_generator.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <fcntl.h>
#include <sys/wait.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>

namespace {

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

}  // namespace

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
