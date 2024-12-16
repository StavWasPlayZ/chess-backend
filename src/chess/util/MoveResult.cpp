#include "MoveResult.h"

using namespace chess;

#define LEGAL_RESULTS_LEN 3

static constexpr chess::MoveResult LEGAL_RESULTS[LEGAL_RESULTS_LEN] = {
    MoveResult::LEGAL_MOVE,
    MoveResult::CHECK,
    MoveResult::CHECKMATE
};

bool chess::isLegal(const MoveResult moveResult)
{
    for (size_t i = 0; i < LEGAL_RESULTS_LEN; i++)
    {
        if (moveResult == LEGAL_RESULTS[i])
        {
            return true;
        }
    }
    
    return false;
}