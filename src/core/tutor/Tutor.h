#pragma once

#include "../student/Student.h"
#include "../domain/Stages.h"

class Tutor
{
public:
    Tutor();

    Stage decideNextStage(
        Stage expected,
        Stage actual,
        const Student& student
    );
};