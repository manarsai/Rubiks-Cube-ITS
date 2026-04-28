#pragma once

#include "Cube.h"
#include "../domain/Stages.h"

// One stage rule
struct StageDefinition
{
    Stage stage;
    const char* name;

    bool (*isComplete)(const Cube& cube);
};

namespace StageDefinitions
{
    const StageDefinition& get(Stage stage);
    Stage detect(const Cube& cube);

    // ? single source of truth functions
    bool isWhiteCrossComplete(const Cube& cube);
    bool isF2LComplete(const Cube& cube);
    bool isOLLComplete(const Cube& cube);
    bool isPLLComplete(const Cube& cube);

    // ? shared validator used internally
    bool validateStage(Stage stage, const Cube& cube);
}