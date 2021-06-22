// C++17

#pragma once

template <unsigned Taille, typename type = unsigned>
struct bits
{
	type Bits : Taille;

	struct bit
	{
		bits<Taille, type>& Bits;
		unsigned Position;

		bit& operator =(bool Flag) noexcept
		{
			if (Flag)
			{
				this->Bits.Bits |= (1 << this->Position);
			}
			else
			{
				this->Bits.Bits &= ~(1 << this->Position);
			}

			return *this;
		}

		operator bool() const noexcept
		{
			return this->Bits.Bits & (1u << Position);
		}

		bit& operator ~() noexcept
		{
			this->Bits.Bits ^= (1u << Position);

			return *this;
		}
	};

	const bit operator [](unsigned Position) const noexcept
	{
		return { const_cast<bits<Taille, type>&> (*this), Position };
	}

	bit operator [](unsigned Position) noexcept
	{
		return { *this, Position };
	}

	bits<Taille, type>& operator |=(const bits<Taille, type>& Bits) noexcept
	{
		this->Bits |= Bits.Bits;

		return *this;
	}

	bits<Taille, type>& operator &=(const bits<Taille, type>& Bits) noexcept
	{
		this->Bits &= Bits.Bits;

		return *this;
	}

	bits<Taille, type>& operator ^=(const bits<Taille, type>& Bits) noexcept
	{
		this->Bits ^= Bits.Bits;

		return *this;
	}

	bits<Taille, type>& operator <<=(unsigned Nombre) noexcept
	{
		this->Bits <<= Nombre;

		return *this;
	}

	bits<Taille, type>& operator >>=(unsigned Nombre) noexcept
	{
		this->Bits >>= Nombre;

		return *this;
	}

	//operator const type&() const noexcept
	//{
	//	return Bits;
	//}

	//operator type& () noexcept
	//{
	//	return Bits;
	//}
};

template <unsigned Taille, typename type>
bool operator ==(const bits<Taille, type>& Bits1, const bits<Taille, type>& Bits2) noexcept
{
	return Bits1.Bits == Bits2.Bits;
}

template <unsigned Taille, typename type>
bool operator !=(const bits<Taille, type>& Bits1, const bits<Taille, type>& Bits2) noexcept
{
	return Bits1.Bits != Bits2.Bits;
}

template <unsigned Taille, typename type>
bits<Taille, type> operator |(const bits<Taille, type>& Bits1, const bits<Taille, type>& Bits2) noexcept
{
	auto Retour{ Bits1 };

	return Retour |= Bits2;
}

template <unsigned Taille, typename type>
bits<Taille, type> operator &(const bits<Taille, type>& Bits1, const bits<Taille, type>& Bits2) noexcept
{
	auto Retour{ Bits1 };

	return Retour &= Bits2;
}

template <unsigned Taille, typename type>
bits<Taille, type> operator ^(const bits<Taille, type>& Bits1, const bits<Taille, type>& Bits2) noexcept
{
	auto Retour{ Bits1 };

	return Retour ^= Bits2;
}

template <unsigned Taille, typename type>
bits<Taille, type> operator <<(const bits<Taille, type>& Bits, unsigned Nombre) noexcept
{
	auto Retour{ Bits };

	return Retour <<= Nombre;
}

template <unsigned Taille, typename type>
bits<Taille, type> operator >>(const bits<Taille, type>& Bits, unsigned Nombre) noexcept
{
	auto Retour{ Bits };

	return Retour >>= Nombre;
}

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

	return retour{ (Bits >> (Taille / 2)).Bits, Bits.Bits };
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
const bits<Taille, type> double_rotation_gauche(const bits<Taille, type>& Bits)
{
	auto [L, R] { couper(Bits) };

	return fusionner(rotation_gauche(L), rotation_gauche(R));
}