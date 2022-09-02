#include "RemovingDuplicates.h"

namespace solution {

void remove_duplicates(char * str)
{
    auto * last = str;
    while (*(str++) != '\0') {
        if (*str != *last) {
            *(++last) = *str;
        }
    }
}

} // namespace solution
