#pragma once

#include "Cube.h"
#include "../domain/Stages.h"

// One stage rule
struct StageDefinition
{
    Stage stage;
    const char* name;

    // returns true when stage is complete
    bool (*isComplete)(const Cube& cube);
};

// Access all stage definitions
namespace StageDefinitions
{
    const StageDefinition& get(Stage stage);

    Stage detect(const Cube& cube);  // ?? ADD THIS
}