#include "convert_command.h"
#include <bits/iterator_concepts.h>
#include <optional>
#include <string>
#include <vector>

bool endswith(const std::string& str, const std::string& suffix)
{
    if (suffix.length() > str.length()) {
        return false;
    }

    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

std::vector<std::string> c_str_array_to_string_vector(const char* const* array)
{
    std::vector<std::string> result;
    for (const char* const* it = array; *it != nullptr; ++it) {
        result.push_back(*it);
    }

    return result;
}

bool is_source_file(const std::string& file_path)
{
    size_t pos = file_path.find_last_of('.');
    if (pos != std::string::npos) {
        std::string extension = file_path.substr(pos + 1);
        return extension == "c" || extension == "cpp" || extension == "cxx";
    }
    return false;
}

std::vector<std::string> resolve_arguments(const std::vector<std::string> arguments)
{
    std::vector<std::string> result;
    for (auto iter = arguments.begin(); iter != arguments.end(); ++iter) {
        if (*iter == "-c" || *iter == "-E") {
            result.push_back("-E");
            continue;
        }

        if (*iter == "-D" || *iter == "-I") {
            result.push_back(*iter);
            ++iter;
            result.push_back(*iter);
            continue;
        }

        if (iter->find("-D") == 0 || iter->find("-I") == 0) {
            result.push_back(*iter);
            continue;
        }
    }

    return result;
}

std::optional<command> convert_command(const char* filename,
    char** arguments,
    char** env)
{
    if (filename == nullptr) {
        return std::nullopt;
    }

    command result;

    result.filename = filename;
    result.arguments = c_str_array_to_string_vector(arguments);
    result.env = c_str_array_to_string_vector(env);

    bool flag = false;
    if (endswith(result.filename, "cc1plus") || endswith(result.filename, "cc1")) {
        result.filename = "clang++";
        result.arguments = resolve_arguments(result.arguments);
        std::vector<std::string> clang_arguments = { "-Xclang", "-ast-dump=json", "-w", "-fsyntax-only" };
        result.arguments.insert(result.arguments.begin(), clang_arguments.begin(), clang_arguments.end());
        return result;
    }

    return std::nullopt;
}
