#include "benchmark_cli.h"
#include "cli_options.h"

#include <serde/serde.h>

#include <string>

namespace ann {

// clang-format off
std::optional<RunArguments> BenchmarkCli::Parse(int argc, char** argv) {
    RunArguments arguments;
    std::string backend;
    std::string mode;

    CliOptions cli(argc, argv);
    cli.AddUsage("Build an ANN index or measure search.");
    cli.AddPositional("backend", "Available options: 'faiss', 'scann', 'hnsw', 'spann'")
       .StoreResult(&backend);
    cli.AddPositional("mode", "Available options: 'build', 'search'").StoreResult(&mode);
    cli.AddOption("--config", "Common build, search and measurement configuration")
       .StoreResult(&arguments.GeneralConfigPath);
    cli.AddOption("--backend-config", "Native configuration for the selected backend")
       .StoreResult(&arguments.BackendConfigPath);
    cli.AddOption("--dataset", "Dataset directory").StoreResult(&arguments.DatasetPath);
    cli.AddOption("--index", "Index directory: build output or search input")
       .StoreResult(&arguments.IndexPath);
    cli.AddOption("--output", "Result JSON file").StoreResult(&arguments.OutputPath);

    if (!cli.Parse()) {
        return std::nullopt;
    }

    arguments.Backend = Serde::Deserialize<BackendKind>(backend);
    arguments.Mode = Serde::Deserialize<RunMode>(mode);
    return arguments;
}

} // namespace ann

//clang-format on
