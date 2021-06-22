// C++17

#pragma once

#include "bits.hpp"

namespace DES
{
	using text = bits<8,unsigned>;
	using key = bits<10,unsigned>;

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

	template <unsigned Taille>
	bits<Taille> remplir(bool(&Tableau)[Taille])
	{
		bits<Taille> Retour{ 0u };

		for (auto i{ 0u }; i < Taille; ++i)
		{
			if (Tableau[i]) Retour.Bits |= (1 << i);
		}

		return Retour;
	}

	// Fonctionne
	bits<8u> ip(const bits<8u>& Bits)
	{
		bool Tableau[]{ Bits[1], Bits[3], Bits[0], Bits[4], Bits[7], Bits[5], Bits[2], Bits[6] };

		return remplir(Tableau);
	}

	// Fonctionne
	bits<8u> ip1(const bits<8u>& Bits)
	{
		bool Tableau[]{ Bits[2], Bits[0], Bits[6], Bits[1], Bits[3], Bits[5], Bits[7], Bits[4] };

		return remplir(Tableau);
	}

	// Fonctionne
	bits<8u> SW(const bits<8u>& Bits)
	{
		auto Copie{ Bits };
		Copie >>= 4;
		return Copie |= (Bits << 4);
	}

	bits<2u> S0[4][4]{ {1u, 0u, 3u, 2u}, {3u, 2u, 1u, 0u}, {0u, 2u, 1u, 3u}, {3u, 1u, 3u, 2u} };
	bits<2u> S1[4][4]{ {0u, 1u, 2u, 3u}, {2u, 0u, 1u, 3u}, {3u, 0u, 1u, 0u}, {2u, 1u, 0u, 3u} };

	// Fonctionne
	auto F(const bits<4u>& Bits, const bits<8u>& K)
	{
		bits<8u> Expansion{ 0u };

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

	bits<8u> f(const bits<8u>& Bits, const bits<8u>& K)
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
}