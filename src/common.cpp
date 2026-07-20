#include "common.h"

Position Position::fromString(const string& s)  {
    if (s.size()!= 2) return {-1  ,-1};
    char col = tolower(s[0]);
    char row = s[1];
    if (col<'a' || col>'h' || row<'1' || row>'8') return {-1,-1};
    return {row-'1', col-'a'};
}
