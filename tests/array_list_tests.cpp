#include <catch2/catch.hpp>

#include <memory>
#include <vector>

#include "element.hpp"
#include "generation.hpp"

#include "array_list.hpp"

using namespace std;
using namespace itis;
using namespace Catch::Matchers;

CATCH_REGISTER_ENUM(Element, Element::CHERRY_PIE, Element::SECRET_BOX, Element::DRAGON_BALL,
                    Element::GRAVITY_GUN, Element::BEAUTIFUL_FLOWERS, Element::UNINITIALIZED)

SCENARIO("create empty array list") {

  GIVEN("no initial capacity") {

    WHEN("using a default constructor") {
      const auto list = make_unique<ArrayList>();

      THEN("list size should be zero") {
        CHECK(list->GetSize() == 0);
      }

      AND_THEN("list capacity should be set to " << ArrayList::kInitCapacity) {
        CHECK(list->GetCapacity() == ArrayList::kInitCapacity);
      }

      AND_THEN("all designated elements should be set to Element::UNINITIALIZED") {
        const auto elements_ref = vector<Element>(ArrayList::kInitCapacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }
  }

  AND_GIVEN("positive initial capacity") {
    const int init_capacity = GENERATE(range(1, 10));

    WHEN("using capacity constructor") {
      const auto list = make_unique<ArrayList>(init_capacity);

      CAPTURE(init_capacity);

      THEN("list size should be zero") {
        CHECK(list->GetSize() == 0);
      }

      AND_THEN("list capacity should be set to the provided initial capacity") {
        CHECK(list->GetCapacity() == init_capacity);
      }

      AND_THEN("all designated elements should be set to Element::UNINITIALIZED") {
        const auto elements_ref = vector<Element>(init_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }
  }

  AND_GIVEN("non-positive initial capacity") {
    const int init_capacity = GENERATE(range(-10, 0));

    WHEN("using capacity constructor") {

      THEN("exception must be thrown") {
        CAPTURE(init_capacity);
        REQUIRE_THROWS(ArrayList(init_capacity), StartsWith("ArrayList::capacity") && EndsWith("must be positive"));
      }
    }
  }
}

SCENARIO("add elements to the array list") {

  GIVEN("empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const auto list = make_unique<ArrayList>(nullptr, 0, init_capacity);

    WHEN("adding multiple elements within initial capacity") {
      const int num_elements = GENERATE_COPY(1, init_capacity);

      vector<Element> elements_ref = utils::generate_elements(num_elements, init_capacity);

      for (const auto e: elements_ref) {
        list->Add(e);
      }

      CAPTURE(init_capacity, num_elements);

      THEN("list size should be equal to the number of added elements") {
        CHECK(list->GetSize() == num_elements);
      }

      AND_THEN("list capacity should stay the same") {
        CHECK(list->GetCapacity() == init_capacity);
      }

      AND_THEN("elements should be added to the back of the list") {
        elements_ref.resize(init_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("adding multiple elements exceeding initial capacity") {
      const int growth_coefficient = GENERATE_COPY(1, ArrayList::kCapacityGrowthCoefficient + 1,
                                                   2 * ArrayList::kCapacityGrowthCoefficient + 1);

      const int num_elements = init_capacity + growth_coefficient;
      const int expected_capacity = num_elements + ArrayList::kCapacityGrowthCoefficient - 1;

      vector<Element> elements_ref = utils::generate_elements(num_elements, num_elements);

      for (const auto e: elements_ref) {
        list->Add(e);
      }

      CAPTURE(init_capacity, num_elements, expected_capacity);

      THEN("list size should be equal to the number of added elements") {
        CHECK(list->GetSize() == num_elements);
      }

      AND_THEN("list capacity should be increased according to the growth coefficient") {
        REQUIRE(list->GetCapacity() == expected_capacity);
      }

      AND_THEN("elements should be added to the back of the list") {
        elements_ref.resize(expected_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }
  }
}

SCENARIO("insert elements into the array list") {

  GIVEN("empty array lists") {
    const int init_capacity = GENERATE(range(2, 10));
    const auto list = make_unique<ArrayList>(nullptr, 0, init_capacity);

    WHEN("inserting elements to the front") {
      const int insert_index = 0;
      const int num_elements_to_insert = GENERATE_COPY(range(1, init_capacity));

      vector<Element> elements_ref = utils::generate_elements(num_elements_to_insert, init_capacity);

      for (const auto e: elements_ref) {
        list->Insert(insert_index, e);
      }

      CAPTURE(init_capacity, num_elements_to_insert, insert_index, elements_ref);

      THEN("list size should be equal to the number of inserted elements") {
        CHECK(list->GetSize() == num_elements_to_insert);
      }

      AND_THEN("list capacity should stay the same") {
        CHECK(list->GetCapacity() == init_capacity);
      }

      AND_THEN("one element should be inserted to the front of the list") {
        std::reverse(elements_ref.begin(), elements_ref.end());
        elements_ref.resize(init_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("inserting element into invalid index") {
      const int insert_index = GENERATE_COPY(range(1, 10), range(-10, 0));

      THEN("exception should be thrown") {
        CAPTURE(init_capacity, insert_index);
        CHECK_THROWS(list->Insert(insert_index, Element::SECRET_BOX), StartsWith("insert_index is out of range"));
      }
    }
  }

  AND_GIVEN("non-empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const int init_num_elements = GENERATE_COPY(range(1, init_capacity));

    vector<Element> elements_ref = utils::generate_elements(init_num_elements, init_capacity);
    const auto list = make_unique<ArrayList>(elements_ref.data(), init_num_elements, init_capacity);

    WHEN("inserting elements into valid positions") {
      const int insert_index = GENERATE_COPY(range(0, init_num_elements + 1));
      const int num_elements_to_insert = GENERATE_COPY(range(init_capacity - init_num_elements, 0, -1));

      const int expected_num_elements = init_num_elements + num_elements_to_insert;

      vector<Element> insert_elements_ref = utils::generate_elements(num_elements_to_insert, num_elements_to_insert);

      for (const auto e: insert_elements_ref) {
        list->Insert(insert_index, e);
      }

      CAPTURE(insert_index, init_capacity, init_num_elements, num_elements_to_insert);

      THEN("list size should be increased to the number of inserted elements") {
        CHECK(list->GetSize() == expected_num_elements);
      }

      AND_THEN("list capacity should stay the same") {
        CHECK(list->GetCapacity() == init_capacity);
      }

      AND_THEN("elements should be inserted list") {
        std::reverse(insert_elements_ref.begin(), insert_elements_ref.end());
        elements_ref.insert(elements_ref.begin() + insert_index,
                            insert_elements_ref.begin(),
                            insert_elements_ref.end());
        elements_ref.resize(init_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("inserting elements into invalid positions") {
      const int index = GENERATE_COPY(range(-10, 0), range(init_num_elements + 1, init_num_elements + 5));

      THEN("exception should be thrown") {
        CAPTURE(index);
        CHECK_THROWS(list->Insert(index, Element::SECRET_BOX));
      }
    }
  }

  AND_GIVEN("array list with size equal to capacity") {
    const int init_capacity = GENERATE(range(2, 10));
    const int num_elements = init_capacity;

    vector<Element> elements_ref = utils::generate_elements(num_elements, init_capacity);
    const auto list = make_unique<ArrayList>(elements_ref.data(), num_elements, init_capacity);

    WHEN("inserting an element into valid position") {
      const int index = GENERATE_COPY(range(0, num_elements + 1));

      const int expected_capacity = init_capacity + ArrayList::kCapacityGrowthCoefficient;
      const auto inserted_element = Element::GRAVITY_GUN;

      CHECK_NOTHROW(list->Insert(index, inserted_element));
      elements_ref.insert(elements_ref.begin() + index, inserted_element);

      CAPTURE(init_capacity, num_elements);

      THEN("list size should be increased") {
        CHECK(list->GetSize() == num_elements + 1);
      }

      AND_THEN("list capacity should be increased according to the growth coefficient") {
        CHECK(list->GetCapacity() == expected_capacity);
      }

      AND_THEN("one element should be inserted at the right position") {
        elements_ref.resize(expected_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }
  }
}

SCENARIO("remove elements from the array list") {

  GIVEN("empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const auto list = make_unique<ArrayList>(nullptr, 0, init_capacity);

    WHEN("removing elements at any index") {
      const int index = GENERATE(range(-5, 5));

      CAPTURE(init_capacity, list->GetSize());

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Remove(index), StartsWith("index is out bounds"));
      }
    }
  }

  AND_GIVEN("non-empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const int num_elements = GENERATE_COPY(range(1, init_capacity));

    vector<Element> elements_ref = utils::generate_elements(num_elements, init_capacity);
    const auto list = make_unique<ArrayList>(elements_ref.data(), num_elements, init_capacity);

    REQUIRE(list->GetSize() == num_elements);
    REQUIRE(list->GetCapacity() == init_capacity);

    WHEN("removing element from list") {
      const int remove_index = GENERATE_COPY(range(0, num_elements));

      const Element removed_element_ref = elements_ref.at(remove_index);
      elements_ref.erase(elements_ref.begin() + remove_index);

      const Element removed_element = list->Remove(remove_index);

      CAPTURE(init_capacity, num_elements, remove_index, removed_element);

      THEN("list capacity should stay the same") {
        CHECK(list->GetCapacity() == init_capacity);
      }

      AND_THEN("list size should be decreased") {
        CHECK(list->GetSize() == num_elements - 1);
      }

      AND_THEN("removed element should be returned") {
        CHECK(removed_element == removed_element_ref);
      }

      AND_THEN("element should be removed from list") {
        elements_ref.resize(init_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("removing element with invalid index") {
      const int remove_index = GENERATE_COPY(range(-5, 0), range(num_elements, init_capacity + 5));

      THEN("exception should be thrown") {
        CAPTURE(init_capacity, num_elements, remove_index);
        CHECK_THROWS(list->Remove(remove_index));
      }
    }
  }
}

SCENARIO("clear array list") {

  GIVEN("empty array list") {
    const auto init_capacity = GENERATE(range(1, 10));
    const auto list = make_unique<ArrayList>(nullptr, 0, init_capacity);

    WHEN("clearing array list") {

      list->Clear();

      THEN("list size should be zero") {
        CHECK(list->GetSize() == 0);
      }

      AND_THEN("list capacity should stay the same") {
        CHECK(list->GetCapacity() == init_capacity);
      }

      AND_THEN("all designated elements should be set to Element::UNINITIALIZED") {
        const vector<Element> elements(init_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements);
      }
    }
  }

  AND_GIVEN("non-empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const int num_elements = GENERATE_COPY(range(1, init_capacity + 1));

    vector<Element> elements_ref = utils::generate_elements(num_elements, init_capacity);
    const auto list = make_unique<ArrayList>(elements_ref.data(), num_elements, init_capacity);

    CAPTURE(init_capacity, num_elements);

    WHEN("clearing array list") {
      list->Clear();

      THEN("list size should become zero") {
        CHECK(list->GetSize() == 0);
      }

      AND_THEN("list capacity should stay the same") {
        CHECK(list->GetCapacity() == init_capacity);
      }

      AND_THEN("all elements should be set to Element::UNINITIALIZED") {
        elements_ref.clear();
        elements_ref.resize(init_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }
  }
}

SCENARIO("access array list elements by index") {

  GIVEN("empty array list") {
    const int init_capacity = GENERATE(range(1, 10));
    const auto list = make_unique<ArrayList>(nullptr, 0, init_capacity);

    WHEN("accessing element at any index") {
      const int index = GENERATE(range(-5, 5));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Get(index));
      }
    }
  }

  AND_GIVEN("non-empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const int num_elements = GENERATE_COPY(range(1, init_capacity + 1));

    vector<Element> elements_ref = utils::generate_elements(num_elements, init_capacity);
    const auto list = make_unique<ArrayList>(elements_ref.data(), num_elements, init_capacity);

    CAPTURE(init_capacity, num_elements);

    WHEN("accessing elements_ref with valid index") {
      const int index = GENERATE_COPY(range(0, num_elements));
      const auto expected_element = elements_ref.at(index);

      THEN("correct element should be returned") {
        CHECK(list->Get(index) == expected_element);
      }
    }

    AND_WHEN("accessing elements_ref with invalid index") {
      const int index = GENERATE_COPY(range(-5, 0), range(num_elements, num_elements + 5));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Get(index), StartsWith("index is out of bounds"));
      }
    }
  }
}

SCENARIO("find array list element index") {

  GIVEN("empty array list") {
    const int init_capacity = GENERATE(range(1, 10));
    const auto list = make_unique<ArrayList>(nullptr, 0, init_capacity);

    WHEN("finding any possible element") {
      const int search_element_id = GENERATE(range(0, static_cast<int>(Element::UNINITIALIZED) - 1));
      const auto search_element = static_cast<Element>(search_element_id);

      const int found_element_index = list->IndexOf(search_element);

      THEN("search should fail") {
        CHECK(found_element_index == ArrayList::kNotFoundElementIndex);
      }
    }
  }

  AND_GIVEN("non-empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const int num_elements = GENERATE_COPY(range(1, init_capacity + 1));

    const int search_element_index = GENERATE_COPY(range(0, num_elements));

    const Element search_element_ref = Element::GRAVITY_GUN;
    vector<Element> elements_ref(num_elements, Element::SECRET_BOX);
    elements_ref.at(search_element_index) = search_element_ref;
    elements_ref.reserve(init_capacity);

    const auto list = make_unique<ArrayList>(elements_ref.data(), num_elements, init_capacity);

    WHEN("finding existing element") {
      const auto search_element = elements_ref.at(search_element_index);
      const int found_element_index = list->IndexOf(search_element);

      THEN("search should return positive index") {
        CHECK((found_element_index >= 0 && found_element_index <= num_elements));
        CHECK(elements_ref.at(found_element_index) == search_element_ref);
      }
    }

    AND_WHEN("finding non-existing element") {
      const auto search_element = Element::DRAGON_BALL;
      const int found_element_index = list->IndexOf(search_element);

      THEN("search should fail with negative index") {
        CHECK(found_element_index == ArrayList::kNotFoundElementIndex);
      }
    }
  }
}

SCENARIO("check if array list contains elements") {

  GIVEN("empty array list") {
    const int init_capacity = GENERATE(1, 10);
    const auto list = make_unique<ArrayList>(nullptr, 0, init_capacity);

    WHEN("checking if list contains any element") {
      const auto element = static_cast<Element>(GENERATE(0, Element::UNINITIALIZED));

      THEN("false should be returned") {
        CHECK_FALSE(list->Contains(element));
      }
    }
  }

  AND_GIVEN("non-empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const int num_elements = GENERATE_COPY(range(1, init_capacity + 1));

    vector<Element> elements_ref(num_elements, Element::DRAGON_BALL);
    elements_ref.reserve(init_capacity);

    WHEN("checking of list contains existing element") {
      const int element_index = GENERATE_COPY(range(0, num_elements));
      const auto element = Element::CHERRY_PIE;

      elements_ref.at(element_index) = element;
      const auto list = make_unique<ArrayList>(elements_ref.data(), num_elements, init_capacity);

      THEN("true should be returned") {
        CHECK(list->Contains(element));
      }
    }

    AND_WHEN("checking of list contains non-existing element") {
      const auto element = Element::CHERRY_PIE;
      const auto list = make_unique<ArrayList>(elements_ref.data(), num_elements, init_capacity);

      THEN("false should be returned") {
        CHECK_FALSE(list->Contains(element));
      }
    }
  }
}

SCENARIO("set array list element value") {

  GIVEN("empty array list") {
    const auto list = make_unique<ArrayList>(nullptr, 0, 1);

    WHEN("setting element value at any index") {
      const int index = GENERATE(range(-10, 10));

      THEN("exception should be thrown") {
        CAPTURE(index);
        CHECK_THROWS(list->Set(index, Element::CHERRY_PIE));
      }
    }
  }

  AND_GIVEN("non-empty array list") {
    const int init_capacity = GENERATE(range(2, 10));
    const int num_elements = GENERATE_COPY(range(1, init_capacity + 1));

    vector<Element> elements_ref = utils::generate_elements(num_elements, init_capacity);
    const auto list = make_unique<ArrayList>(elements_ref.data(), num_elements, init_capacity);

    WHEN("setting element value at valid index") {
      const int index = GENERATE_COPY(range(0, num_elements));
      const auto element = Element::SECRET_BOX;

      CHECK_NOTHROW(list->Set(index, element));
      elements_ref.at(index) = element;

      THEN("element at index should be set") {
        elements_ref.resize(init_capacity, Element::UNINITIALIZED);
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("setting element value at invalid index") {
      const int index = GENERATE_COPY(range(-5, 0), range(num_elements, num_elements + 5));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Set(index, Element::CHERRY_PIE));
      }
    }
  }
}
