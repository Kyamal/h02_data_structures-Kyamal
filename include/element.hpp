#pragma once

namespace itis {

// перечисление: элементы списка
enum class Element {
  CHERRY_PIE,
  SECRET_BOX,
  DRAGON_BALL,
  GRAVITY_GUN,
  BEAUTIFUL_FLOWERS,
  UNINITIALIZED  // специальное значение, обозначающее отсутствие элемента
};

// внутренние проверки
static_assert(static_cast<int>(Element::UNINITIALIZED) == 5, "Enum class Element contains too many values");

}  // namespace itis