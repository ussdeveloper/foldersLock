// Standard C++ libraries
#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cmath>      // For std::pow
#include <iomanip>    // For std::setprecision, std::fixed, std::left, std::setw
#include <sstream>    // For std::ostringstream
#include <algorithm>  // For std::min

// Filesystem namespace alias
namespace fs = std::filesystem;

// Forward declaration
void printProgress(long long dirsCreated, long long totalDirsToCreate, int currentProcessingLevel, int overallMaxDepth);

/**
 * @brief Recursively creates a directory structure.
 *
 * For the given path and depth, this function creates 10 subdirectories (0-9)
 * and calls itself for each, until the maximum depth is reached.
 * It also updates a progress counter.
 *
 * @param currentPath The current path where subdirectories are being created.
 * @param currentDepth The current recursion depth (of the parent).
 * @param maxDepth The maximum target depth of the directory structure (recursion stops when currentDepth == maxDepth).
 * @param dirsCreated Reference to the counter for successfully created/processed directories.
 * @param totalDirsToCreate The total number of directories expected to be created.
 * @param overallMaxDepth The initial maximum depth requested by the user (for display purposes).
 */
void createDirectoriesRecursively(const fs::path& currentPath, int currentDepth, int maxDepth, 
                                  long long& dirsCreated, long long totalDirsToCreate, int overallMaxDepth) {
    // Base case: if current depth (of parent) has reached maxDepth, then children would be beyond maxDepth.
    // Or, more directly, if the level of directories to be created (currentDepth + 1) is greater than maxDepth.
    // The loop for subdirectories runs for currentDepth from 0 to maxDepth-1.
    if (currentDepth >= maxDepth) { // currentDepth is 0 for first level, up to maxDepth-1 for last level of parents
        return;
    }

    for (int i = 0; i < 10; ++i) {
        fs::path newDir = currentPath / std::to_string(i);
        bool can_recurse = false;

        try {
            if (fs::create_directory(newDir)) {
                can_recurse = true;
            } else {
                if (fs::exists(newDir) && fs::is_directory(newDir)) {
                    can_recurse = true;
                } else {
                    std::cerr << "\nWarning: Could not create or access '" << newDir.string() << "' as a directory." << std::endl;
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "\nError creating or accessing directory " << newDir.string() << ": " << e.what() << std::endl;
        }

        if (can_recurse) {
            dirsCreated++;
            // currentDepth + 1 is the level of the directory just created (newDir)
            printProgress(dirsCreated, totalDirsToCreate, currentDepth + 1, overallMaxDepth);
            createDirectoriesRecursively(newDir, currentDepth + 1, maxDepth, dirsCreated, totalDirsToCreate, overallMaxDepth);
        }
    }
}

/**
 * @brief Prints the progress of directory creation.
 *
 * @param dirsCreated Number of directories created so far.
 * @param totalDirsToCreate Total number of directories to create.
 * @param currentProcessingLevel The current depth level being processed (1 to overallMaxDepth).
 * @param overallMaxDepth The maximum depth of the operation.
 */
void printProgress(long long dirsCreated, long long totalDirsToCreate, int currentProcessingLevel, int overallMaxDepth) {
    if (overallMaxDepth == 0) return; // No progress to show if depth is 0
    // If totalDirsToCreate is 0 but depth > 0, it's an issue, but avoid division by zero.
    // However, totalDirsToCreate should be correctly calculated if depth > 0.
    if (totalDirsToCreate == 0 && overallMaxDepth > 0) { 
        // This state indicates an issue with totalDirsToCreate calculation if depth > 0
        // For safety, one might print a static message or just return.
        // For now, we proceed, but percentage calculation might be off.
    }

    long long displayDirsCreated = std::min(dirsCreated, totalDirsToCreate);
    double percentage = (totalDirsToCreate == 0) ? 100.0 : (static_cast<double>(displayDirsCreated) / totalDirsToCreate * 100.0);
    
    int barWidth = 20; 
    int filledChars = (totalDirsToCreate == 0) ? barWidth : static_cast<int>((static_cast<double>(displayDirsCreated) / totalDirsToCreate) * barWidth);

    std::ostringstream oss;
    // currentProcessingLevel is the level of directories being worked on (1-indexed)
    oss << "Creating (Depth " << overallMaxDepth << "): [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < filledChars) oss << "#";
        else oss << "-";
    }
    oss << "] " << std::fixed << std::setprecision(1) << percentage << "% (" 
        << displayDirsCreated << "/" << totalDirsToCreate << ")";

    std::cout << "\r" << std::left << std::setw(80) << oss.str() << std::flush;
}

/**
 * @brief Main function.
 *
 * Parses command-line arguments, validates them, and initiates directory creation.
 */
int main(int argc, char* argv[]) {
    std::string rootDirName;
    int depth = 4; // Default depth

    if (argc < 2) {
        std::cerr << "Usage: " << (argc > 0 ? argv[0] : "folders-lock.exe") << " <root_directory_name_or_path> [depth]" << std::endl;
        std::cerr << "  <root_directory_name_or_path>: Mandatory. The name of or path to the root directory." << std::endl;
        std::cerr << "  [depth]: Optional. The number of subdirectory levels. Default is " << depth << "." << std::endl;
        std::cerr << "\nExample usage:" << std::endl;
        std::cerr << "  " << (argc > 0 ? argv[0] : "folders-lock.exe") << " my_data_directory" << std::endl;
        std::cerr << "  " << (argc > 0 ? argv[0] : "folders-lock.exe") << " C:\\\\path\\\\to\\\\directory 3" << std::endl;
        return 1;
    }

    rootDirName = argv[1];
    if (argc > 2) {
        depth = std::atoi(argv[2]);
        if (depth <= 0) {
            std::cerr << "Error: Depth must be a positive integer. Using default depth: " << 4 << std::endl;
            depth = 4;
        }
    }
    // Ensure depth is not negative for calculations, though previous check handles depth <= 0.
    // If depth is 0, no subdirectories will be created.
    if (depth < 0) depth = 0; 

    long long totalDirsToCreate = 0LL;
    if (depth > 0) {
        for (int d_level = 1; d_level <= depth; ++d_level) {
            totalDirsToCreate += static_cast<long long>(std::pow(10, d_level));
        }
    }
    long long dirsCreated = 0LL;

    fs::path rootPath = rootDirName;
    try {
        if (fs::create_directory(rootPath)) {
            std::cout << "Created root directory: " << rootPath.string() << std::endl;
        } else {
            if (fs::exists(rootPath) && fs::is_directory(rootPath)) {
                 std::cout << "Root directory '" << rootPath.string() << "' already exists. Proceeding to create subdirectories." << std::endl;
            } else if (fs::exists(rootPath)) {
                std::cerr << "Error: Path '" << rootPath.string() << "' exists but is not a directory." << std::endl;
                return 1;
            } else {
                std::cerr << "Error: Could not create root directory '" << rootPath.string() << "' and it does not exist." << std::endl;
                return 1;
            }
        }
        
        if (fs::exists(rootPath) && fs::is_directory(rootPath)) {
            if (depth > 0) { 
                // Initial progress display: processing level 1 out of 'depth' total levels.
                printProgress(0, totalDirsToCreate, 1, depth);
            }
            // Start recursion: currentDepth is 0 (for root's children), maxDepth is 'depth', overallMaxDepth is 'depth'.
            createDirectoriesRecursively(rootPath, 0, depth, dirsCreated, totalDirsToCreate, depth);
            
            if (totalDirsToCreate > 0 || depth == 0) { // Ensure newline even if depth was 0 and no progress bar shown
                // Final progress update to 100% or actual if errors occurred.
                // currentProcessingLevel is 'depth' as we are conceptually done with all levels up to 'depth'.
                if (depth > 0) printProgress(dirsCreated, totalDirsToCreate, depth, depth); 
                std::cout << std::endl; 
            }
            std::cout << "Directory structure successfully created in '" << rootPath.string() << "' with depth " << depth << "." << std::endl;
        } else if (!fs::exists(rootPath)) { 
             std::cerr << "Error: Failed to create root directory: " << rootPath.string() << std::endl;
             return 1;
        }

    } catch (const fs::filesystem_error& e) {
        if (depth > 0) std::cout << std::endl; // Newline if progress was shown before error
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}