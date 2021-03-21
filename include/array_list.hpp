#pragma once

#include <ostream>
#include <vector>

#include "element.hpp"  // Element

namespace itis {

/**
 * Структура данных "массив переменной длины".
 *
 * Характеризуется своей емкостью (capacity) и кол-ом элементов (size).
 * Все элементы массива находятся последовательно в памяти,
 * что обеспечивает получение за O(1).
 *
 * Пример:
 * [1 2 3 x x x x x]
 * Емкость массива = 8
 * Кол-во элементов = 3
 * x - ячейки памяти под элементы (пустые / без определенного значения)
 */
struct ArrayList {
 public:
  // константы структуры
  static constexpr int kInitCapacity = 10;               // изначальная емкость массива [МОЖНО ИЗМЕНЯТЬ]
  static constexpr int kCapacityGrowthCoefficient = 10;  // коэфициент увеличения размера массива [МОЖНО ИЗМЕНЯТЬ]
  static constexpr int kNotFoundElementIndex = -1;       // индекс ненайденного элемента в массиве

 private:
  // поля структуры
  int size_{0};             // размер (кол-во реальных элементов в массиве)
  int capacity_{0};         // емкость (кол-во ячеек памяти под элементы в массиве)
  Element *data_{nullptr};  // указатель на начало непрерывного блока памяти под элементы

 public:
  // конструктор по умолчанию
  ArrayList();

  /**
   * Создание массива определенной емкости.
   *
   * Выделенные ячейки массива инициализируются значением Element::UNINITIALIZED.
   * [x x x x x x x], где x = Element::UNINITIALIZED, size = 0, capacity = 7,
   *
   * @param capacity - начальная емкость массива
   * @throws invalid_argument при указании неположительной емкости
   */
  explicit ArrayList(int capacity);

  // деструктор
  virtual ~ArrayList();

  /**
   * Добавление элемента в конец массива ~ O(1)/O(n).
   *
   * При недостаточной емкости осуществляется расширение емкости массива с сохранением элементов.
   * [1 2 3] => add(4) => [1 2 3 4 x x x]
   * {capacity = 3, size = 3} => {capacity = 7, size = 4}
   *
   * @param e - значение элемента
   */
  void Add(Element e);

  /**
   * Вставка элемента в массив по индексу ~ O(n).
   *
   * Все элементы, находящиеся на позиции вставки и справа от нее, сдвигаются на единицу вправо.
   * При недостаточной емкости осуществляется расширение емкости массива с сохранением элементов.
   * [1 2 3 x] => insert(1, 4) => [1 4 2 3]
   * {capacity = 4, size = 3} => {capacity = 4, size = 4}
   *
   * @param index - позиция для вставки элемента
   * @param e - значение элемента
   *
   * @throws out_of_range при передаче индекса за пределами массива
   */
  void Insert(int index, Element e);

  /**
   * Изменение значения элемента массива по индексу ~ O(1).
   *
   * @param index - индекс изменяемого элемента массива
   * @param value - новое значение элемента
   *
   * @throws out_of_range при передаче индекса за пределами массива
   */
  void Set(int index, Element value);

  /**
   * Удаление элемента массива по индексу ~ O(n).
   *
   * Все элементы, стоящие справа от удаленного элемента сдвигаются влево на единицу.
   * Освободившиаяся ячейка массива инициализируется значением Element::UNINITIALIZED.
   * [1 2 3] => remove(0) => [2 3 x]
   * {capacity = 3, size = 3} => {capacity = 3, size = 2}
   *
   * @param index - индекс удаляемого элемента
   * @return значение удаленного элемента
   *
   * @throws out_of_range при передаче индекса за пределами массива
   */
  Element Remove(int index);

  /**
   * Очистка массива ~ O(n).
   *
   * Емкость массива остается прежней.
   * Все освободившиеся ячейки устанавливаются в значение Element::UNINITIALIZED.
   */
  void Clear();

  /**
   * Получение элемента массива по индексу ~ O(1).
   *
   * @param index - индекс элемента
   * @return значение элемента по индексу
   *
   * @throws out_of_range при передаче индекса за пределами массива
   *
   */
  Element Get(int index) const;

  /**
   * Поиск индекса первого вхождения элемента с указанным значением ~ O(n).
   *
   * @param e - значение элемента
   * @return индекс элемента или -1 при остутствии элемента в массиве
   */
  int IndexOf(Element e) const;

  bool Contains(Element e) const;

  int GetSize() const;

  int GetCapacity() const;

  bool IsEmpty() const;

 private:

  /**
   * Увеличение емкости массива ~ O(n).
   *
   * @param new_capacity - новая емкость массива (должна быть больше предыдущей)
   */
  void resize(int new_capacity);

 public:
  // необходимо для тестирования
  ArrayList(Element *data, int size, int capacity);
  friend std::ostream &operator<<(std::ostream &, const ArrayList &);
  friend bool operator==(const ArrayList &, const std::vector<Element> &);
};

// внутренние проверки
static_assert(ArrayList::kInitCapacity > 0, "ArrayList initial capacity must be positive");
static_assert(ArrayList::kCapacityGrowthCoefficient > 1, "ArrayList growth coefficient must be greater than 1");

}  // namespace itis
