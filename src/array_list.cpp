#include "array_list.hpp"  // подключаем заголовочный файл с объявлениями

#include <algorithm>  // copy, fill
#include <cassert>    // assert
#include <stdexcept>  // out_of_range, invalid_argument

#include "private/internal.hpp"  // вспомогательные функции

namespace itis {

ArrayList::ArrayList(int capacity) : capacity_{capacity} {
  if (capacity <= 0) {
    throw std::invalid_argument("ArrayList::capacity must be positive");
  }

  // Tip 1: используйте std::fill для заполнения выделенных ячеек массива значением Element::UNINITIALIZED
  // здесь должен быть ваш код ...
}

ArrayList::~ArrayList() {
  // Tip 1: высвободите выделенную память
  // Tip 2: не забудьте про логическую целостность объекта (инвариантность)
}

void ArrayList::Add(Element e) {
  // Tip 1: используйте метод resize(new_capacity) для расширения емкости массива
  // здесь должен быть ваш код ...

  assert(size_ < capacity_);  // я здесь, чтобы не дать тебе сойти с правильного пути

  // напишите свой код после расширения емкости массива здесь ...
}

void ArrayList::Insert(int index, Element e) {
  if (index != 0 && index != size_) {
    // index = 0 и index == size это особые случаи, при которых всегда можно выполнить операцию вставки
    internal::check_out_of_range(index, 0, size_);
  }

  // Tip 1: используйте метод resize(new_capacity) для расширения емкости массива
  // напишите свой код здесь ...

  assert(size_ < capacity_);  // я ни в коем случае не дам вам совершить ошибку всей вашей жизни

  // Tip 2: для свдига элементов вправо можете использовать std::copy
  // напишите свой код после расширения емкости массива здесь ...
}

void ArrayList::Set(int index, Element value) {
  internal::check_out_of_range(index, 0, size_);
  // напишите свой код здесь ...
}

Element ArrayList::Remove(int index) {
  internal::check_out_of_range(index, 0, size_);

  // Tip 1: можете использовать std::copy для сдвига элементов влево
  // Tip 2: не забудьте задать значение Element::UNINITIALIZED освободившейся ячейке
  // напишите свой код здесь ...
  return {};
}

void ArrayList::Clear() {
  // Tip 1: можете использовать std::fill для заполнения ячеек массива значением  Element::UNINITIALIZED
  // напишите свой код здесь ...
}

Element ArrayList::Get(int index) const {
  internal::check_out_of_range(index, 0, size_);
  // напишите свой код здесь ...
  return {};
}

int ArrayList::IndexOf(Element e) const {
  // напишите свой код здесь ...
  return {};
}

// === РЕАЛИЗОВАНО ===

bool ArrayList::Contains(Element e) const {
  // здесь был Рамиль
  return IndexOf(e) != kNotFoundElementIndex;
}

// это делегирующий конструктор если что
ArrayList::ArrayList() : ArrayList(kInitCapacity) {}

int ArrayList::GetSize() const {
  return size_;
}

int ArrayList::GetCapacity() const {
  return capacity_;
}

bool ArrayList::IsEmpty() const {
  return size_ == 0;
}

// Легенда: давным давно на планете под названием Земля жил да был Аватар...
// Аватар мог управлять четырьмя стихиями, но никак не мог совладать с C++ (фейспалм).
// Помогите найти непростительную ошибку Аватара,
// которая привела к гибели десятков тысяч котиков (плак-плак, шмыгание носом, втягивание соплей).
// P.S. кол-во ошибок может быть более одной, порядку операций можно верить
void ArrayList::resize(int new_capacity) {
  assert(new_capacity > capacity_);  // не ошибается тот, кто ничего не делает ...

  // 1. выделяем новый участок памяти
  auto new_data = new Element[capacity_];

  // 2. копируем данные на новый участок
  std::copy(data_, data_ + size_ - 1, new_data);

  // 3. заполняем "свободные" ячейки памяти значением Element::UNINITIALIZED
  std::fill(new_data + size_, new_data + new_capacity, Element::UNINITIALIZED);

  // 4. высвобождаем старый участок памяти меньшего размера
  delete data_;

  // 5. пересылаем указатель на новый участок памяти
  data_ = new_data;

  // 6. не забываем посолить ... кхм... обновить емкость массива
  capacity_ = new_capacity;
}

// === ЗОНА 51: необходимо для тестирования ===

ArrayList::ArrayList(Element *data, int size, int capacity) : size_{size}, capacity_{capacity} {
  assert(capacity > 0 && size >= 0 && size <= capacity);

  data_ = new Element[capacity];
  std::fill(data_, data_ + capacity, Element::UNINITIALIZED);

  if (data != nullptr) {
    std::copy(data, data + size, data_);
  }
}

std::ostream &operator<<(std::ostream &os, const ArrayList &list) {
  if (list.data_ != nullptr) {
    os << "{ ";
    for (int index = 0; index < list.capacity_ - 1; index++) {
      os << internal::elem_to_str(list.data_[index]) << ", ";
    }
    os << internal::elem_to_str(list.data_[list.capacity_ - 1]) << " }";
  } else {
    os << "{ nullptr }";
  }
  return os;
}

bool operator==(const ArrayList &list, const std::vector<Element> &elements) {
  if (list.data_ == nullptr) return false;
  if (list.capacity_ != static_cast<int>(elements.size())) return false;

  for (int index = 0; index < list.capacity_; index++) {
    if (list.data_[index] != elements.at(index)) return false;
  }
  return true;
}

}  // namespace itis