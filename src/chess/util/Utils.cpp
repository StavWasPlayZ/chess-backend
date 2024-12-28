#include "Utils.h"

bool chess::util::mirrorCheck(int index, int goal, int size)
{
    return (index == goal) || ((index + 1) == (size - goal));
}