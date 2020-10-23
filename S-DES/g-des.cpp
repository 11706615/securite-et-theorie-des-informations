#include <iostream>
#include <bitset>

struct dix
{
	unsigned int Bits : 10;
};

#define bit_est_actif(Bits, Position) (Bits & (1u << Position))
#define changer_bit(Bits, Position) (Bits ^= (1u << Position))

dix p10(const dix& Dix)
{
	dix Resultat{ 0u };

	if (bit_est_actif(Dix.Bits, 0)) changer_bit(Resultat.Bits, 4);
	if (bit_est_actif(Dix.Bits, 1)) changer_bit(Resultat.Bits, 2);
	if (bit_est_actif(Dix.Bits, 2)) changer_bit(Resultat.Bits, 1);
	if (bit_est_actif(Dix.Bits, 3)) changer_bit(Resultat.Bits, 6);
	if (bit_est_actif(Dix.Bits, 4)) changer_bit(Resultat.Bits, 0);
	if (bit_est_actif(Dix.Bits, 5)) changer_bit(Resultat.Bits, 8);
	if (bit_est_actif(Dix.Bits, 6)) changer_bit(Resultat.Bits, 5);
	if (bit_est_actif(Dix.Bits, 7)) changer_bit(Resultat.Bits, 9);
	if (bit_est_actif(Dix.Bits, 8)) changer_bit(Resultat.Bits, 7);
	if (bit_est_actif(Dix.Bits, 9)) changer_bit(Resultat.Bits, 3);

	return Resultat;
}


struct cinq
{
	unsigned int a : 5;
	unsigned int b : 5;
};

cinq rotation_gauche(const cinq& Cinq)
{
	return { (Cinq.a << 1) | (Cinq.a >> 4), (Cinq.b << 1) | (Cinq.b >> 4) };
}

cinq dix_vers_cinq(const dix& Dix)
{
	return { Dix.Bits >> 5, Dix.Bits };
}

struct huit
{
	unsigned Bits : 8;
};

huit p8(const dix& Dix)
{
	huit Resultat{ 0u };

	if (bit_est_actif(Dix.Bits, 0)) changer_bit(Resultat.Bits, 1);
	if (bit_est_actif(Dix.Bits, 1)) changer_bit(Resultat.Bits, 0);
	if (bit_est_actif(Dix.Bits, 2)) changer_bit(Resultat.Bits, 3);
	if (bit_est_actif(Dix.Bits, 3)) changer_bit(Resultat.Bits, 5);
	if (bit_est_actif(Dix.Bits, 4)) changer_bit(Resultat.Bits, 7);
	if (bit_est_actif(Dix.Bits, 5)) changer_bit(Resultat.Bits, 2);
	if (bit_est_actif(Dix.Bits, 6)) changer_bit(Resultat.Bits, 4);
	if (bit_est_actif(Dix.Bits, 7)) changer_bit(Resultat.Bits, 6);

	return Resultat;
}

cinq MAX{ 1u << 4u, 1u << 4u };

#define move(bit, taille) (bit = (bit & (1u << (taille))) ? (bit << 1) + 1 : (bit << 1))

int main()
{
	dix Dix{ 0b0000111000 };

	std::cout << std::bitset <10>(Dix.Bits) << std::endl;
	std::cout << std::bitset <8>(p8(Dix).Bits) << std::endl;

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