#include <cinttypes>
#include <cstddef>
template <typename derived_type, size_t size>
class Alphabet
{
  static_assert(size > 0, "alphabet size must be > 0");

  constexpr Alphabet(void) noexcept = default;
  constexpr Alphabet(Alphabet const &) noexcept = default;
  constexpr Alphabet(Alphabet &&) noexcept = default;
  constexpr Alphabet & operator=(Alphabet const &) noexcept = default;
  constexpr Alphabet & operator=(Alphabet &&) noexcept = default;
  ~Alphabet(void) noexcept = default; 
  constexpr char to_char(uint8_t rank) const noexcept
  {
    return derived_type::rank_to_char(rank);
  }
};
