#include "cli_options.h"

#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <utility>

namespace ann {

// clang-format off
CliOptions::CliOptions(int argc, char** argv)
    : ProgramName_(std::filesystem::path(argv[0]).filename().string()) {
    for (int i = 1; i < argc; ++i) {
        Tokens_.emplace_back(argv[i]);
    }
}

CliOptions& CliOptions::AddUsage(std::string description) {
    Description_ = std::move(description);
    return *this;
}

CliOptions& CliOptions::AddOption(std::string name, std::string help) {
    Specs_.push_back({std::move(name), std::move(help), false, true, {}});
    return *this;
}

CliOptions& CliOptions::AddPositional(std::string name, std::string help) {
    Specs_.push_back({std::move(name), std::move(help), true, true, {}});
    return *this;
}

CliOptions& CliOptions::Required() {
    Specs_.back().Required = true;
    return *this;
}

CliOptions& CliOptions::Optional() {
    Specs_.back().Required = false;
    return *this;
}

bool CliOptions::Parse() {
    for (const auto& token : Tokens_) {
        if (token == "--help" || token == "-h") {
            PrintHelp();
            return false;
        }
    }

    std::vector<bool> seen(Specs_.size(), false);

    for (std::size_t i = 0; i < Tokens_.size(); ++i) {
        const std::string_view token = Tokens_[i];
        const auto equal = token.find('=');
        const auto name = token.substr(0, equal);

        std::size_t specIndex = 0;

        while (specIndex < Specs_.size()) {
            const auto& spec = Specs_[specIndex];
            if (token.starts_with('-') ? !spec.Positional && spec.Name == name : spec.Positional && !seen[specIndex]) {
                break;
            }
            ++specIndex;
        }

        if (specIndex == Specs_.size()) {
            throw std::invalid_argument("unknown argument: " + std::string(token));
        }
        const auto& spec = Specs_[specIndex];

        if (seen[specIndex]) {
            throw std::invalid_argument("duplicate option: " + spec.Name);
        }
        seen[specIndex] = true;

        std::string_view value = token;
        if (!spec.Positional) {
            if (equal != std::string_view::npos) {
                value = token.substr(equal + 1);
            } else {
                if (i + 1 == Tokens_.size() || Tokens_[i + 1].starts_with("--")) {
                    throw std::invalid_argument("option " + spec.Name + " requires a value");
                }
                value = Tokens_[++i];
            }
        }

        try {
            spec.Assign(value);
        } catch (const std::exception& error) {
            throw std::invalid_argument(
                "invalid value for " + spec.Name + ": '"
                + std::string(value) + "' (" + error.what() + ")"
            );
        }
    }

    for (std::size_t i = 0; i < Specs_.size(); ++i) {
        if (Specs_[i].Required && !seen[i]) {
            throw std::invalid_argument("missing required argument: " + Specs_[i].Name);
        }
    }

    return true;
}

std::string CliOptions::Label(const Spec& spec) {
    return spec.Positional
        ? "<" + spec.Name + ">"
        : spec.Name + " <value>";
}

void CliOptions::PrintHelp() const {
    std::cout << "Usage: " << ProgramName_;
    std::size_t width = std::string_view("--help, -h").size();

    for (const auto& spec : Specs_) {
        const auto label = Label(spec);

        std::cout << ' ' << (spec.Required ? label : "[" + label + "]");
        width = std::max(width, label.size());
    }

    std::cout
        << " [--help]\n\n"
        << Description_
        << "\n\n";

    for (const auto& spec : Specs_) {
        std::cout
            << "  " << std::left << std::setw(static_cast<int>(width))
            << Label(spec) << "  "
            << spec.Help
            << (spec.Required ? " (required)" : "")
            << '\n';
    }

    std::cout
        << "  " << std::left << std::setw(static_cast<int>(width))
        << "--help, -h"
        << "  Show this help\n";
}

} // namespace ann

// clang-format on
