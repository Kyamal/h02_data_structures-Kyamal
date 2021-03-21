#pragma once

#include <ostream>
#include <vector>

#include "element.hpp"  // Element

namespace itis {

/**
 * Структура "узел".
 * Хранит в себе данные и указатель на следующий узел.
 */
struct Node {
 public:
  // поля структуры
  Element data{Element::UNINITIALIZED};
  Node *next{nullptr};

  // конструктор
  Node(Element e, Node *ptr) : data{e}, next{ptr} {}
};

/**
 * Структура данных "связный список".
 *
 * Хранит в себе цепочку узлов со значениями элементов.
 * Характеризуется своим размером (кол-ом элементов).
 * Дополнительно хранит в себе указатели на первый и последний узел.
 */
struct LinkedList {
 public:
  static constexpr int kNotFoundElementIndex = -1;  // индекс ненайденного элемента в списке

 private:
  // поля структуры
  int size_{0};          // кол-во узлов в списке
  Node *head_{nullptr};  // первый узел
  Node *tail_{nullptr};  // последний узел

 public:
  // конструктор по умолчанию
  // Прим. ключевое слово default говорит компилятору сгенирировать конструктор самостоятельно
  LinkedList() = default;

  // деструктор
  virtual ~LinkedList();

  /**
   * Добавление элемента в конец списка ~ O(1).
   *
   * Элемент "оборачивается" в узел и добавляется в конец списка,
   * т.е. предыдущий последний узел указывает на только что созданный узел.
   * 1 -> 2 -> 3 -> nullptr => add(4) => 1 -> 2 -> 3 -> 4 -> nullptr
   * {size = 3, head = 1, tail = 3} => {size = 4, head = 1, tail = 4}
   *
   * @param e - значение элемента
   */
  void Add(Element e);

  /**
   * Вставка элемента в список по индексу ~ O(n).
   * Прим. вставка в конец или начало списка ~ O(1).
   *
   * Все элементы, находящиеся на позиции вставки и справа от нее, сдвигаются вправо.
   * 1 -> 2 -> 3 -> nullptr => insert(1, 7) => 1 -> 7 -> 2 -> 3 -> nullptr
   * {size = 3, head = 1, tail = 3} => {size = 4, head = 1, tail = 3}
   *
   * @param index - позиция для вставки элемента
   * @param e - значение элемента
   *
   * @throws out_of_range при передаче индекса за пределами списка
   */
  void Insert(int index, Element e);

  /**
   * Изменение значения элемента списка по индексу ~ O(n).
   *
   * @param index - индекс изменяемого элемента списка
   * @param e - новое значение элемента
   *
   * @throws out_of_range при передаче индекса за пределами списка
   */
  void Set(int index, Element e);

  /**
   * Удаление элемента списка по индексу ~ O(n).
   * Прим. при удалении элемента с начала списка ~ O(1).
   *
   * Все элементы, стоящие справа от удаленного элемента сдвигаются влево.
   * 1 -> 2 -> 3 -> nullptr => remove(1) => 1 -> 3 -> nullptr
   * {size = 3, head = 1, tail = 3} => {size = 2, head = 1, tail = 3}
   *
   * @param index - индекс удаляемого элемента
   * @return значение удаленного элемента
   *
   * @throws out_of_range при передаче индекса за пределами массива
   */
  Element Remove(int index);

  /**
   * Удаление всех элементов списка ~ O(n).
   *
   * Происходит высвобождение памяти, выделенной под узлы списка (эквивалетно деструктору).
   * 1 -> 2 -> 3 -> nullptr => nullptr
   * {size = 3, head = 1, tail = 3} => {size = 0, head = nullptr, tail = nullptr}
   */
  void Clear();

  /**
   * Получение элемента списка по индексу ~ O(n).
   *
   * @param index - индекс элемента
   * @return значение элемента по индексу
   *
   * @throws out_of_range при передаче индекса за пределами списка
   */
  Element Get(int index) const;

  /**
   * Поиск индекса первого вхождения элемента с указанным значением ~ O(n).
   *
   * @param e - значение элемента
   * @return индекс элемента или -1 при остутствии элемента в списке
   */
  int IndexOf(Element e) const;

  bool Contains(Element e) const;

  int GetSize() const;

  bool IsEmpty() const;

  Element tail() const;

  Element head() const;

 private:

  /**
   * Поиск узла по индексу ~ O(n).
   *
   * @param index - индекс элемента
   * @return указатель на узел
   */
  Node *find_node(int index) const;

 public:
  // необходимо для тестирования
  explicit LinkedList(const std::vector<Element> &);
  friend std::ostream &operator<<(std::ostream &, const LinkedList &);
  friend bool operator==(const LinkedList &, const std::vector<Element> &);
};

}  // namespace itis
