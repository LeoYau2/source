#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

using namespace std
namespace fs = std::filesystem;

std::mutex cout_mutex;
std::atomic<int> count(0);
int countAll = 0;

void process_file(const fs::path& input_file, const fs::path& output_file, const fs::path& temp_file, int z_factor) {
    // Copy file to temp directory
    fs::copy_file(input_file, temp_file, fs::copy_options::overwrite_existing);

    // Construct command
    std::wstring command = L"C:\\Users\\Leo\\webp2\\cwebp.exe -lossless -z " + std::to_wstring(z_factor) + L" \"" + temp_file.wstring() + L"\" -o \"" + output_file.wstring() + L"\"";

    // Execute command
    FILE* pipe = _wpopen(command.c_str(), L"r");
    if (!pipe) {
        std::cerr << "Error executing command" << std::endl;
        return;
    }
    _pclose(pipe);

    // Copy metadata
    auto input_time = fs::last_write_time(input_file);
    fs::last_write_time(output_file, input_time);

    // Remove temp file
    fs::remove(temp_file);

    // Update progress
    int current_count = count++;
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
    double progress = static_cast<double>(current_count) / countAll * 100;
    int eta_seconds = static_cast<int>((duration / current_count) * (countAll - current_count));

    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Progress: " << current_count << "/" << countAll << ", " << std::fixed << std::setprecision(2) << progress << "%" << std::endl;
    std::cout << "Elapsed: " << duration / 3600 << ":" << (duration % 3600) / 60 << ":" << duration % 60;
    std::cout << ", ETA: " << eta_seconds / 3600 << ":" << (eta_seconds % 3600) / 60 << ":" << eta_seconds % 60 << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

int main() {
    std::wstring input_container = L"png";
    int maxConcurrency = 8;
    int z_factor = 8;

    std::wcout << L"Enter maxConcurrency (default: 8): ";
    std::wstring input;
    std::getline(std::wcin, input);
    if (!input.empty()) {
        maxConcurrency = std::stoi(input);
    }

    std::wcout << L"Enter z factor (compression 1~9, default: 8): ";
    std::getline(std::wcin, input);
    if (!input.empty()) {
        z_factor = std::stoi(input);
    }

    std::wcout << L"z : " << z_factor << std::endl;
    system("pause");

    fs::path root = fs::current_path();
    fs::path dir = root / (L"webp.z" + std::to_wstring(z_factor));
    fs::path tempDir = root / L"temp_processing";

    fs::create_directories(dir);
    fs::create_directories(tempDir);

    std::vector<fs::path> files;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (entry.is_regular_file() && entry.path().extension() == L"." + input_container) {
            files.push_back(entry.path());
        }
    }

    countAll = files.size();

    auto start_time = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < files.size(); ++i) {
        const auto& file = files[i];
        fs::path relativePath = fs::relative(file, root);
        fs::path outputPath = dir / relativePath.parent_path() / (relativePath.stem().wstring() + L".webp");
        fs::path tempOutputPath = tempDir / (std::to_wstring(i) + L".webp");
        fs::path tempFilePath = tempDir / (std::to_wstring(i) + L"." + input_container);

        fs::create_directories(outputPath.parent_path());

        threads.emplace_back(process_file, file, outputPath, tempFilePath, z_factor);

        if (threads.size() >= maxConcurrency) {
            for (auto& t : threads) {
                t.join();
            }
            threads.clear();
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    fs::remove_all(tempDir);

    auto end_time = std::chrono::steady_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    std::cout << "Total processing time: " << total_time / 3600 << ":" << (total_time % 3600) / 60 << ":" << total_time % 60 << std::endl;

    return 0;
}
