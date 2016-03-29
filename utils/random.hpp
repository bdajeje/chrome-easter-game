#ifndef UTILS_RANDOM_HPP
#define UTILS_RANDOM_HPP

namespace utils {

template <class T>
T random(T min, T max)
{
  return rand() % max + min;
}

}

#endif // UTILS_RANDOM_HPP
