#pragma once

// ЕСЛИ ВЫ ЗДЕСЬ, ЗНАЧИТ ЛЮБОПЫТСТВО ВЗЯЛО НАД ВАМИ ВЕРХ.
// КАК ЭНЕКИН СКАЙУОКЕР ЖЕЛАЛ ЗАПОЛУЧИТЬ СИЛУ ДЛЯ ЗАЩИТЫ СВОИХ БЛИЗКИХ,
// НО СОШЕЛ С ПРАВИЛЬНОГО ПО МНЕНИЮ БОЛЬШИНСТВА ЛЮДЕЙ В ЭТОМ МИРЕ ПРАВИЛЬНОГО ПУТИ,
// ТАК И ВЫ ВОЗЖЕЛАЛИ ОВЛАДЕТЬ ВСЕМОГУЩЕЙ СИЛОЙ C++ ДЛЯ СДАЧИ ЭКЗАМЕНА ПО АиСД.
// ВАС НЕ ОСТАНОВИТ НИЧТО НА ЭТОМ ПУТИ, ПОЭТОМУ ДЛЯ ВАС Я ОСТАВИЛ КОММЕНТАРИИ.
// ВОЗРАДУЙТЕСЬ, ИБО БЕЗГРАНИЧНАЯ СИЛА ПОЗНАНИЯ НАПОЛНЯЕТ НАШИ ПЫЛАЮЩИЕ СЕРДЦА...
// P.S. Я писал это в 2:36 МСК, простите меня

#include <sstream>
#include <stdexcept>
#include <string_view>

#include "element.hpp"

namespace itis::internal {

/**
 * Проверка выхода значения за указанные пределы.
 *
 * @param index - проверяемое значение
 * @param min - минимальное допустимое значение (включительно)
 * @param max - максимальное допустимое значение (не включительно)
 *
 * @throws ошибку out_of_range при выходе за указанные границы
 */
inline void check_out_of_range(int index, int min, int max) {
  if (index >= min && index < max) return;

  std::stringstream ss("index is out of range: ");
  ss << '[' << min << ',' << max << ']';
  throw std::out_of_range(ss.str());
}

/**
 * Отображение перечисления Element в строковое представление.
 * P.S. думаю, что это происходит в момент компиляции.
 *
 * @param e - перечислитель
 * @return строковое представление перечислителя
 */
inline constexpr std::string_view elem_to_str(Element e) {
  switch (e) {
    case Element::CHERRY_PIE:return "CHERRY_PIE";
    case Element::BEAUTIFUL_FLOWERS:return "BEAUTIFUL_FLOWERS";
    case Element::DRAGON_BALL:return "DRAGON_BALL";
    case Element::GRAVITY_GUN:return "GRAVITY_GUN";
    case Element::SECRET_BOX:return "SECRET_BOX";
    default:return "UNINITIALIZED";
  }
}

}  // namespace itis::internal