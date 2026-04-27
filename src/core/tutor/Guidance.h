/*GUIDANCE.CPP
TURN DECISION INTO EXPLAINS
DECIDES LEVEL OF DETAIL





*/



/*
TUTOR.cpp
DECIDE THE NEXT STAGE
ADJUSTDIFFICULTYL

*/

/*
TUTORCONTROLLER.CPP
RECEIVES THE SCAN
ASKS DOMAIN TO INTERPRET, COMPARE SUBMISSION TO SOLUTION
UPDATE STUDENT MODEL
ASKS ENGINE TO DECIDE NEXT STAGE
ASKS INSTRUCTION TO EXPLAIN


receive scan
ask domain for the actualstage
compare domain info with given info (success or failure
update student model
engine decides based on success history 
instructiion format input


PROCESS SCAN
RECORDATTEMPT
SETSTAGE
SETINSTRUCTION

*/

#pragma once

#include <string>
#include "../student/Student.h"
#include "../domain/Stages.h"

class Guidance
{
public:
    std::string generate(Stage stage, const Student& student);

    std::string generateHint(Stage stage, const Student& student);
};