// C++17

#include <iostream>

#include "g-des.hpp"

template <unsigned Taille, typename type>
std::ostream& operator <<(std::ostream& Stream, const bits<Taille, type>& Bits)
{
	for (auto i{ 1u }; i <= Taille; ++i)
	{
		Stream << Bits[Taille - i] ? "1" : "0";
	}

	return Stream;
}

int main()
{
	DES::text Text{ 0b01000111 };
	DES::key Key{ 0b0000111000 };

	std::cout << Text << std::endl;
	std::cout << DES::encrypt(Text, Key) << std::endl;
	std::cout << DES::decrypt(DES::encrypt(Text, Key), Key);
}