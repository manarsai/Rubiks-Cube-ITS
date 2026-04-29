#pragma once

#include "../domain/types.h"
#include "../domain/Stages.h"
#include "../domain/Colour.h"

namespace StageDefinitions
{
    struct StageDefinition
    {
        Stage stage;
        const char* name;

        bool (*isComplete)(const State& state);
    };

    // core API
    const StageDefinition& get(Stage stage);
    Stage detect(const State& state);

    bool validateStage(Stage stage, const State& state);

    // checks
    bool isWhiteCrossComplete(const State& state);
    bool isF2LComplete(const State& state);
    bool isOLLComplete(const State& state);
    bool isPLLComplete(const State& state);
}