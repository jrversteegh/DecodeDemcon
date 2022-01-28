#include <iostream>
#include <vector>

void printGeneration(std::vector<bool> const aGeneration);

int main()
{
    std::vector<bool> myGeneration;

    printGeneration(myGeneration);
    return 0;
}

void printGeneration(std::vector<bool> const aGeneration)
{
    for (bool i : aGeneration)
    {
        if (i)
        {
            std::cout << '*';
        }
        else
        {
            std::cout << ' ';
        }
    }
    std::cout << std::endl;
}
