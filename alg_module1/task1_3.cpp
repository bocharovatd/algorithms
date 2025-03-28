// Во всех задачах необходимо использование битовых операций.

// Если в числе содержится только один бит со значением 1, записать в выходной поток OK.
// Иначе записать FAIL.
// Необходимо использование битовых операций.
// Использование арифметических операций запрещено.

#include <cassert>
#include <iostream>
#include <sstream>

bool is_pow_of_two(unsigned int n) {
    while ((n != 0) & ((n & 1) != 1)) {
        n = n >> 1;
    }
    return (n == 1);
}

void run(std::istream &input, std::ostream &output) {
    unsigned int n = 0;
    input >> n;
    assert(n >= 0 && n < (static_cast<unsigned long int>(1) << 32));
    output << (is_pow_of_two(n) ? "OK" : "FAIL");
}

void test_bin() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "0";
        run(input, output);
        assert(output.str() == "FAIL");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1";
        run(input, output);
        assert(output.str() == "OK");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "8";
        run(input, output);
        assert(output.str() == "OK");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7";
        run(input, output);
        assert(output.str() == "FAIL");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "9";
        run(input, output);
        assert(output.str() == "FAIL");
    }
}

int main() {
    run(std::cin, std::cout);
    //test_bin();
    return 0;
}
