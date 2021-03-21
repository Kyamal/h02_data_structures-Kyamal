#include <catch2/catch.hpp>

#include <memory>
#include <vector>

#include "element.hpp"
#include "generation.hpp"

#include "linked_list.hpp"

using namespace std;
using namespace itis;
using namespace Catch::Matchers;

SCENARIO("create empty linked list") {

  WHEN("constructing an empty list") {
    const auto list = make_unique<LinkedList>();

    THEN("list size should be zero") {
      CHECK(list->GetSize() == 0);
      CHECK(list->IsEmpty());
    }

    AND_THEN("list head and tail should be uninitialized") {
      CHECK(list->head() == Element::UNINITIALIZED);
      CHECK(list->tail() == Element::UNINITIALIZED);
    }
  }
}

SCENARIO("add elements to the linked list") {

  GIVEN("empty empty list") {
    const auto list = make_unique<LinkedList>();

    REQUIRE(list->IsEmpty());
    REQUIRE(list->tail() == Element::UNINITIALIZED);
    REQUIRE(list->head() == Element::UNINITIALIZED);

    WHEN("adding multiple elements") {
      const int num_elements = GENERATE(1, 10);
      vector<Element> elements_ref = utils::generate_elements(num_elements, num_elements);

      for (const auto e: elements_ref) {
        list->Add(e);
      }

      CAPTURE(elements_ref);

      THEN("list size should increase up to the number of added elements") {
        CHECK(list->GetSize() == num_elements);
      }

      AND_THEN("elements should be added to the list") {
        CHECK(*list == elements_ref);
      }
    }
  }
}

SCENARIO("insert elements to the linked list") {

  GIVEN("empty linked list") {
    const auto list = make_unique<LinkedList>();

    REQUIRE(list->IsEmpty());
    REQUIRE(list->tail() == Element::UNINITIALIZED);
    REQUIRE(list->head() == Element::UNINITIALIZED);

    WHEN("inserting multiple elements") {
      const int index = GENERATE(range(1, 10));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Insert(index, Element::CHERRY_PIE), StartsWith("index is out of bounds"));
      }
    }
  }

  AND_GIVEN("non-empty linked list") {
    const int init_list_size = GENERATE(range(1, 10));

    vector<Element> elements_ref = utils::generate_elements(init_list_size, init_list_size + 1);
    const auto list = make_unique<LinkedList>(elements_ref);

    WHEN("inserting new elements") {
      const int index = GENERATE_COPY(range(0, init_list_size + 1));
      const auto element = Element::SECRET_BOX;

      CHECK_NOTHROW(list->Insert(index, element));
      elements_ref.insert(elements_ref.begin() + index, element);

      CAPTURE(index, elements_ref);

      THEN("list size should be increased") {
        CHECK(list->GetSize() == init_list_size + 1);
      }

      AND_THEN("elements should be correctly inserted") {
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("inserting new elements into invalid indices") {
      const int index = GENERATE_COPY(range(-10, 0), range(init_list_size + 1, init_list_size + 10));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Insert(index, Element::DRAGON_BALL), StartsWith("index is out of bounds"));
      }
    }
  }
}

SCENARIO("set linked list element") {

  GIVEN("empty linked list") {
    const auto list = make_unique<LinkedList>();

    REQUIRE(list->IsEmpty());
    REQUIRE(list->tail() == Element::UNINITIALIZED);
    REQUIRE(list->head() == Element::UNINITIALIZED);

    WHEN("setting element at any index") {
      const int index = GENERATE(range(0, 10));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Set(index, Element::SECRET_BOX), StartsWith("index is out of bounds"));
      }
    }
  }

  AND_GIVEN("non-empty linked list") {
    const int list_size = GENERATE(range(1, 10));

    vector<Element> elements_ref = utils::generate_elements(list_size, list_size);
    const auto list = make_unique<LinkedList>(elements_ref);

    WHEN("setting list elements") {
      const int index = GENERATE_COPY(range(0, list_size));
      const auto element = Element::DRAGON_BALL;

      CHECK_NOTHROW(list->Set(index, element));
      elements_ref.at(index) = element;

      THEN("list size should stay the same") {
        CHECK(list->GetSize() == list_size);
      }

      AND_THEN("list elements should change") {
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("setting list elements at invalid indices") {
      const int index = GENERATE_COPY(range(-10, 0), range(list_size, list_size + 10));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Set(index, Element::BEAUTIFUL_FLOWERS), StartsWith("index is out of bounds"));
      }
    }
  }
}

SCENARIO("remove linked list elements") {

  GIVEN("empty linked list") {
    const auto list = make_unique<LinkedList>();

    REQUIRE(list->IsEmpty());
    REQUIRE(list->tail() == Element::UNINITIALIZED);
    REQUIRE(list->head() == Element::UNINITIALIZED);

    WHEN("removing element at any index") {
      const int index = GENERATE(range(0, 10));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Remove(index), StartsWith("index is out of bounds"));
      }
    }
  }

  AND_GIVEN("non-empty linked list") {
    const int list_size = GENERATE(range(1, 10));

    vector<Element> elements_ref = utils::generate_elements(list_size, list_size);
    const auto list = make_unique<LinkedList>(elements_ref);

    WHEN("removing list elements") {
      const int index = GENERATE_COPY(range(0, list_size));

      const Element remove_element_ref = elements_ref.at(index);
      elements_ref.erase(elements_ref.begin() + index);

      const Element removed_element = list->Remove(index);

      THEN("list size should be decreased") {
        CHECK(list->GetSize() == list_size - 1);
      }

      AND_THEN("removed element should be returned") {
        CHECK(removed_element == remove_element_ref);
      }

      AND_THEN("element should be removed from list") {
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("removing list elements at invalid indices") {
      const int index = GENERATE_COPY(range(-10, 0), range(list_size, list_size + 10));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Remove(index), StartsWith("index is out of bounds"));
      }
    }
  }
}

SCENARIO("clear linked list") {

  GIVEN("empty linked list") {
    const auto list = make_unique<LinkedList>();

    REQUIRE(list->IsEmpty());
    REQUIRE(list->head() == Element::UNINITIALIZED);
    REQUIRE(list->tail() == Element::UNINITIALIZED);

    WHEN("clearing the list") {
      list->Clear();

      THEN("list size be empty") {
        REQUIRE(list->IsEmpty());
        REQUIRE(list->head() == Element::UNINITIALIZED);
        REQUIRE(list->tail() == Element::UNINITIALIZED);
      }
    }
  }

  AND_GIVEN("non-empty linked list") {
    const int list_size = GENERATE(range(1, 10));

    const vector<Element> elements_ref = utils::generate_elements(list_size, list_size);
    const auto list = make_unique<LinkedList>(elements_ref);

    WHEN("clearing the list") {
      list->Clear();

      THEN("list should become empty") {
        REQUIRE(list->IsEmpty());
        REQUIRE(list->head() == Element::UNINITIALIZED);
        REQUIRE(list->tail() == Element::UNINITIALIZED);
      }
    }
  }
}

SCENARIO("get linked list element by index") {

  GIVEN("empty linked list") {
    const auto list = make_unique<LinkedList>();

    REQUIRE(list->IsEmpty());
    REQUIRE(list->tail() == Element::UNINITIALIZED);
    REQUIRE(list->head() == Element::UNINITIALIZED);

    WHEN("getting element at any index") {
      const int index = GENERATE(range(0, 10));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Get(index), StartsWith("index is out of bounds"));
      }
    }
  }

  AND_GIVEN("non-empty linked list") {
    const int list_size = GENERATE(range(1, 10));

    const vector<Element> elements_ref = utils::generate_elements(list_size, list_size);
    const auto list = make_unique<LinkedList>(elements_ref);

    WHEN("getting access elements") {
      const int index = GENERATE_COPY(range(0, list_size));

      const Element element = list->Get(index);
      const Element element_ref = elements_ref.at(index);

      THEN("element should be returned") {
        CHECK(element == element_ref);
      }

      AND_THEN("list should stay the same") {
        CHECK(*list == elements_ref);
      }
    }

    AND_WHEN("getting access to elements at invalid indices") {
      const int index = GENERATE_COPY(range(-10, 0), range(list_size, list_size + 10));

      THEN("exception should be thrown") {
        CHECK_THROWS(list->Get(index), StartsWith("index is out of bounds"));
      }
    }
  }
}

SCENARIO("find linked list elements") {

  GIVEN("empty linked list") {
    const auto list = make_unique<LinkedList>();

    REQUIRE(list->IsEmpty());
    REQUIRE(list->tail() == Element::UNINITIALIZED);
    REQUIRE(list->head() == Element::UNINITIALIZED);

    WHEN("finding any element") {
      const int element_id = GENERATE(range(0, static_cast<int>(Element::UNINITIALIZED)));
      const auto element = static_cast<Element>(element_id);

      THEN("element should not be found") {
        CHECK(list->IndexOf(element) == LinkedList::kNotFoundElementIndex);
      }
    }
  }

  AND_GIVEN("non-empty linked list") {
    const int list_size = GENERATE(range(1, 10));
    const int index_ref = GENERATE_COPY(range(0, list_size));

    const Element element = Element::SECRET_BOX;
    vector<Element> elements_ref(list_size, Element::UNINITIALIZED);
    elements_ref.at(index_ref) = element;

    const auto list = make_unique<LinkedList>(elements_ref);

    WHEN("finding element index") {
      const int index = list->IndexOf(element);

      THEN("correct index should be found") {
        CHECK(index == index_ref);
      }
    }

    AND_WHEN("finding non-existing element") {
      const int index = list->IndexOf(Element::CHERRY_PIE);

      THEN("element should not be found") {
        CHECK(index == LinkedList::kNotFoundElementIndex);
      }
    }
  }
}
