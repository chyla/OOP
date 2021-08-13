#include <iostream>
#include <string>
#include <fstream>
#include <memory>

class NumbersContainer {
public:
    using size_type = int;
    using value_type = int;

    NumbersContainer(size_type max_size) 
      : max_size(max_size),
        count(0),
        numbers(new value_type[max_size])
    {
    }

    void add(value_type value) {
        numbers[count] = value;
        count++;
    }

    void print_content() {
        for (size_type i = 0; i < count; ++i) {
            std::cout << "Content pos: " << i << " value: " << numbers[i] << '\n';
        }
    }

    ~NumbersContainer() {
        delete [] numbers;
    }

private:
    size_type max_size;
    size_type count;
    value_type *numbers;
};


int main() {
    NumbersContainer containerA(10);
    containerA.add(1);
    containerA.add(3);
    containerA.add(2);

    containerA.print_content();

    return 0;
}
