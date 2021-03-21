#pragma once

#include <vector>
#include <random>  // random_device, mt19937, uniform_int_distribution

#include "element.hpp"

namespace utils {

/**
 * Генерация псевдо-случайного массива перечислителей определенного размера и емкости.
 *
 * @param num_samples - кол-во элементов (размер массива)
 * @param capacity - емкость массива
 * @return сгенерированный массив
 */
inline std::vector<itis::Element> generate_elements(int num_samples, int capacity) {
  assert(num_samples <= capacity);

  // устройство генерации псевдо-случайных 32-битных чисел
  auto engine = std::mt19937(std::random_device{}());

  // оболочка над устройством выше для генерации равновероятных значений в указанном диапозоне
  // dist ~ distribution ~ распределение
  auto dist = std::uniform_int_distribution<>(0, static_cast<int>(itis::Element::UNINITIALIZED) - 1);

  std::vector<itis::Element> elements(num_samples);
  elements.reserve(capacity);

  for (int index = 0; index < num_samples; index++) {
    const int enum_id = dist(engine);  // генерация числа
    elements[index] = static_cast<itis::Element>(enum_id);
  }

  return elements;
}

}  // namespace utils