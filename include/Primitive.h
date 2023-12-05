#pragma once
#include "intepreter.h"

class Primitive : public Expression
{
    /*
    type: Primitive
    value type: add/sub/mul/div, get, let, reponse,if-expression 
    
    
    */
private:
    /* data */

public:
    void execute() override
    {
        // Implementation of execute for Primitive
    }
};