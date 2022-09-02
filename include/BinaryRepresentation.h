#include <iostream>

namespace solution {

/**
 * Outputs the binary representation of a number to the provided stream.
 *
 * @param value integer whose binary representation is to be printed.
 * @param stream stream to output binary representation.
 */
template <typename IntegerT>
void print_binary_representation(const IntegerT & value, std::ostream & stream = std::cout)
{
    for (int digit = (sizeof(value) << 3) - 1; digit >= 0; --digit) {
        stream << ((value >> digit) & 1);
    }
}

} // namespace solution
