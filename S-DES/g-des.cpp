#include <iostream>
#include <bitset>

#include "bits.hpp"


// Fonctionne
template <unsigned Taille, typename type>
auto couper(const bits<Taille, type>& Bits)
{
	static_assert(Taille % 2 == 0);

	struct retour
	{
		bits<Taille / 2, type> Premier;
		bits<Taille / 2, type> Deuxieme;
	};

	return	retour{ (Bits >> (Taille / 2)).Bits, Bits.Bits };
}

// Fonctionne
template <unsigned Taille, typename type>
bits<Taille * 2, type> fusionner(const bits<Taille, type>& Premier, const bits<Taille, type>& Deuxieme)
{
	bits<Taille * 2, type> Retour{ Premier.Bits };
	Retour <<= Taille;
	Retour.Bits |= Deuxieme.Bits;
	return Retour;
}

// Fonctionne
template <unsigned Taille, typename type>
bits<Taille, type> rotation_gauche(const bits<Taille, type>& Bits)
{
	return (Bits << 1) | (Bits >> (Taille - 1));
}

// Fonctionne
template <unsigned Taille, typename type>
const bits<Taille,type> double_rotation_gauche(const bits<Taille,type>& Bits)
{
	auto [L, R] { couper(Bits) };

	return fusionner(rotation_gauche(L), rotation_gauche(R));
}



template <unsigned Taille, typename type>
std::ostream& operator <<(std::ostream& Stream, const bits<Taille, type>& Bits)
{
	return Stream << std::bitset <Taille>(Bits.Bits);
}

// Fonctionne
bits<8u> p8(const bits<10u>& Dix)
{
	bits<8u> Resultat;

	Resultat[1] = Dix[0];
	Resultat[0] = Dix[1];
	Resultat[3] = Dix[2];
	Resultat[5] = Dix[3];
	Resultat[7] = Dix[4];
	Resultat[2] = Dix[5];
	Resultat[4] = Dix[6];
	Resultat[6] = Dix[7];

	return Resultat;
}

// Fonctionne
bits<10u> p10(const bits<10u>& Dix)
{
	bits<10u> Resultat{ 0u };

	if (Dix[0]) ~(Resultat[4]);
	if (Dix[1]) ~(Resultat[2]);
	if (Dix[2]) ~(Resultat[1]);
	if (Dix[3]) ~(Resultat[6]);
	if (Dix[4]) ~(Resultat[0]);
	if (Dix[5]) ~(Resultat[8]);
	if (Dix[6]) ~(Resultat[5]);
	if (Dix[7]) ~(Resultat[9]);
	if (Dix[8]) ~(Resultat[7]);
	if (Dix[9]) ~(Resultat[3]);

	return Resultat;
}

template <typename type = unsigned, unsigned Taille>
bits<Taille, type> remplir(bool(&Tableau)[Taille])
{
	bits<Taille, type> Retour{ 0u };

	for (auto i{ 0u }; i < Taille; ++i)
	{
		if (Tableau[i]) Retour.Bits |= (1 << i);
	}

	return Retour;
}

// Fonctionne
template <typename type>
bits<8u, type> ip(const bits<8u, type>& Bits)
{
	bool Tableau[]{ Bits[1], Bits[3], Bits[0], Bits[4], Bits[7], Bits[5], Bits[2], Bits[6] };

	return remplir<type>(Tableau);
}

// Fonctionne
template <typename type>
bits<8u, type> ip1(const bits<8u, type>& Bits)
{
	bool Tableau[]{ Bits[2], Bits[0], Bits[6], Bits[1], Bits[3], Bits[5], Bits[7], Bits[4] };

	return remplir<type>(Tableau);
}

// Fonctionne
template <typename type>
bits<8u, type> SW(const bits<8u, type>& Bits)
{
	auto Copie{ Bits };
	Copie >>= 4;
	return Copie |= (Bits << 4);
}

bits<2u> S0[4][4]{ {1u, 0u, 3u, 2u}, {3u, 2u, 1u, 0u}, {0u, 2u, 1u, 3u}, {3u, 1u, 3u, 2u} };
bits<2u> S1[4][4]{ {0u, 1u, 2u, 3u}, {2u, 0u, 1u, 3u}, {3u, 0u, 1u, 0u}, {2u, 1u, 0u, 3u} };


template <typename type>
auto F(const bits<4u, type>& Bits, const bits<8u, type>& K)
{
	bits<8u, type> Expansion{ 0u };

	if (Bits[0])
	{
		~(Expansion[1]);
		~(Expansion[7]);
	}
	if (Bits[1])
	{
		~(Expansion[2]);
		~(Expansion[4]);
	}
	if (Bits[2])
	{
		~(Expansion[3]);
		~(Expansion[5]);
	}
	if (Bits[3])
	{
		~(Expansion[0]);
		~(Expansion[6]);
	}

	Expansion ^= K;

	unsigned LS0{ (Expansion[7] ? 2u : 0u) + (Expansion[4] ? 1u : 0u) };
	unsigned CS0{ (Expansion[6] ? 2u : 0u) + (Expansion[5] ? 1u : 0u) };
	unsigned LS1{ (Expansion[3] ? 2u : 0u) + (Expansion[1] ? 1u : 0u) };
	unsigned CS1{ (Expansion[2] ? 2u : 0u) + (Expansion[0] ? 1u : 0u) };

	auto res{ fusionner(S0[LS0][CS0], S1[LS1][CS1]) };

	bits <4u> Retour{ 0u };

	if (res[0]) ~(Retour[2]);
	if (res[1]) ~(Retour[1]);
	if (res[2]) ~(Retour[3]);
	if (res[3]) ~(Retour[0]);

	return res;
}

template <typename type>
bits<8u, type> f(const bits<8u, type>& Bits, const bits<8u, type>& K)
{
	auto [L, R] {couper(Bits)};

	return fusionner(L ^ F(R, K), R);
}

bits<8u> encrypt(const bits<8u>& Text, const bits<10u>& Key)
{
	auto K1{ p8(double_rotation_gauche(p10(Key))) };
	auto K2{ p8(rotation_gauche(rotation_gauche(p10(Key)))) };
	
	return ip1(f(SW(f(ip(Text), K1)), K2));
}

bits<8u> decrypt(const bits<8u>& Text, const bits<10u>& Key)
{
	auto K1{ p8(rotation_gauche(p10(Key))) };
	auto K2{ p8(rotation_gauche(rotation_gauche(p10(Key)))) };

	return ip1(f(SW(f(ip(Text), K2)), K1));
}


int main()
{
	

	//std::cout << S0[0][2];

	bits<8u> Text{ 0b01000111 };
	bits<10u> Key{ 0b0000111000 };

	std::cout << double_rotation_gauche(Key);

	std::cout << Text << std::endl;
	std::cout << encrypt(Text, Key) << std::endl;
	std::cout << decrypt(encrypt(Text, Key), Key);


	//std::cout << E << std::endl;
	//std::cout << SW(E);

	//bits <4u>a{ 0b0011u }, b{ 0b1100u };
	//bits <8u>c{ 0b11110000u };

	//f<unsigned int>(a, b, c);

	bits <5u> D{ 0b10100 };

	std::cout << D << std::endl;
	std::cout << rotation_gauche(D) << std::endl;

	bits <10u> Dix{ 0b0000111000 };

	auto [a, b]{ couper(Dix) };



	std::cout << Dix << std::endl;
	std::cout << a << " " << b << std::endl;
	std::cout << fusionner(rotation_gauche(a), rotation_gauche(b)) << std::endl;

	std::cout << p8(Dix) << std::endl;

	bits <8u> Dix2{ 0b00111000 };

	std::cout << Dix2 << std::endl;
	std::cout << ip(Dix2) << std::endl;

	//dix Dix{ 0b0000111000 };

	//std::cout << std::bitset <10>(Dix.Bits) << std::endl;
	//std::cout << std::bitset <8>(p8(Dix).Bits) << std::endl;

	//std::cout << std::bitset <10>(Dix.Bits) << std::endl;
	//std::cout << std::bitset <10>(p10(Dix).Bits) << std::endl;
	//std::cout << std::bitset <5>(dix_vers_cinq(p10(Dix)).a) << ' ' << std::bitset <5>(dix_vers_cinq(p10(Dix)).b) << std::endl;
	//std::cout << std::bitset <5>(rotation_gauche(dix_vers_cinq(p10(Dix))).a) << ' ' << std::bitset <5>(rotation_gauche(dix_vers_cinq(p10(Dix))).b) << std::endl;



	//cinq c{ 30u, 78u };

	////c.a << 1;
	//std::cout << std::bitset <5>(c.a) << ' ' << std::bitset <5>(c.b) << std::endl;
	//std::cout << std::bitset <5>(MAX.a) << ' ' << std::bitset <5>(MAX.b) << std::endl;

	//move(c.a, 4);

	//if (c.a & MAX.a)
	//{
	//	c.a = (c.a << 1) + 1;
	//}
	//else
	//{
	//	c.a <<= 1;
	//}

	//std::cout << std::bitset <5>(c.a) << ' ' << std::bitset <5>(c.b) << std::endl;
}