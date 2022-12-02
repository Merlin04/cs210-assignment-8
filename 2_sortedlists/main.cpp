#include <iostream>

// Write the code for a SortedList class. This class should contain a linked
// list of strings in ascending sorted order. There are two possible ways the
// list can be sorted:
// 1. By string length.
// 2. By individual characters. Starting with the first, one character from
//    each string is compared using the numeric value of the ASCII encoding.
//    If the strings have the same character, compare them on the next.
// Ties can be broken in any order.
// Your class should have the following functionality:

// We will use the stl linked list class
#include <list>

class SortedList {
public:
    std::list<std::string> list;
    bool byLength;
    // a. The ability to create new instances of the class that represent
    //     sorted lists.
    SortedList(): byLength(false) {}

    // b. The ability to create (deep) copies of a SortedList.
    // TODO test that copy is deep for list
    SortedList(const SortedList& other) = default;

    // c. The ability to create new instances of the class that specify
    //    which sorting method to use.
    explicit SortedList(bool byLength): byLength(byLength) {}

    // d. A method to add elements to the list in sorted order.

    [[nodiscard]] int comparator(const std::string& a, const std::string& b) const {
        if(byLength) {
            auto a_len = a.length();
            auto b_len = b.length();
            return a_len > b_len ? 1 : a_len < b_len ? -1 : 0;
        }
        return a > b ? 1 : a < b ? -1 : 0;
    }

    void add(const std::string& s) {
        auto it = list.begin();
        while(it != list.end() && comparator(*it, s) < 0) {
            it++;
        }
        list.insert(it, s);
    }

    // e. A method to remove all instances of an element from the list.
    void remove(const std::string& s) {
        list.remove(s);
    }

    // f. A method to remove all values from the list.
    void clear() {
        list.clear();
    }

    // g. A method that returns whether or not the list is currently
    //    sorted by string length.
    [[nodiscard]] bool isSortedByLength() const {
        return byLength;
    }

    // h. A method that returns whether or not the list is currently
    //    sorted by individual characters.
    [[nodiscard]] bool isSortedByCharacters() const {
        return !byLength;
    }

    // i. A method that returns the length of the list.
    [[nodiscard]] size_t length() const {
        return list.size();
    }

    // j. A method that returns an array containing the values in the
    //    list in current sorted order.
    [[nodiscard]] std::string* toArray() const {
        auto arr = new std::string[list.size()];
        auto i = 0;
        for(const auto& s : list) {
            arr[i++] = s;
        }
        return arr;
    }

    // k. A method that returns the value in the i-th position in current
    //    sorted order.
    [[nodiscard]] std::string get(size_t i) const {
        auto it = list.begin();
        while(i-- > 0) {
            it++;
        }
        return *it;
    }

    // l. A method that changes the list to be sorted by string length.
    void resort() {
        list.sort([this](const std::string& a, const std::string& b) {
            return comparator(a, b) < 0;
        });
    }

    void sortByLength() {
        byLength = true;
        resort();
    }

    // m. A method that changes the list to be sorted by individual
    //    characters.
    void sortByCharacters() {
        byLength = false;
        resort();
    }

    // n. The class should not have any memory leaks.
    // o. The class should have functions properly marked as constant.
};

int main() {
    // Test all of the methods of the SortedList class.

    // a.
    SortedList list;

    // d.
    list.add("hello");
    list.add("world");

    // b.
    SortedList list2 = list;
    list.add("a!");
    std::cout << list2.get(0) << std::endl; // should be "hello"
    std::cout << list.get(0) << std::endl; // should be "a!"

    // c.
    SortedList list3(true);
    list3.add("helloooo");
    list3.add("world");
    std::cout << list3.get(0) << std::endl; // should be "world"

    // e.
    list3.add("world");
    list3.remove("world");
    std::cout << list3.length() << std::endl; // should be 1

    // f.
    list3.clear();
    std::cout << list3.length() << std::endl; // should be 0

    // g.
    std::cout << list3.isSortedByLength() << std::endl; // should be true

    // h.
    std::cout << list3.isSortedByCharacters() << std::endl; // should be false

    // i.
    // already tested

    // j.
    list3.add("helloooooo");
    list3.add("testing");
    list3.add("aaaaaaaaaaaaaa");
    auto arr = list3.toArray();
    // print out arr
    for(auto i = 0; i < list3.length(); i++) {
        std::cout << arr[i] << std::endl;
    }
    // should be "testing", "helloooooo", "aaaaaaaaaaaaaa"
    delete[] arr;

    // k.
    // already tested

    // m.
    list3.sortByCharacters();
    // print it out
    for(auto i = 0; i < list3.length(); i++) {
        std::cout << list3.get(i) << std::endl;
    }
    // should be "aaaaaaaaaaaaaa", "helloooooo", "testing"

    // l.
    list3.sortByLength();
    // print it out
    for(auto i = 0; i < list3.length(); i++) {
        std::cout << list3.get(i) << std::endl;
    }
    // should be "testing", "helloooooo", "aaaaaaaaaaaaaa"

    return 0;
}
