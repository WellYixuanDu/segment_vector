#include "segment_vector.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class TestNode {
public:
  TestNode() = default;
  TestNode(int v, std::string str) : m_value(v), m_str(str) {}
  TestNode(const TestNode &other)
      : m_value(other.m_value), m_str(other.m_str) {}
  void print() {
    std::cout << "The m_value is " << m_value << " The m_str is " << m_str
              << std::endl;
  }
  ~TestNode() = default;

public:
  int m_value;
  std::string m_str;
};

#define CHECK(expr) assert(expr)

void Test_segment_vector_allocator() {
  std::cout
      << "Test segment_vector(const allocator_type& alloc = allocator_type())"
      << std::endl;
  segment_vector<TestNode> tmp;
  CHECK(tmp.size() == 0);
}

void Test_segment_vector_with_size() {
  std::cout << "Test segment_vector(size_type n, const allocator_type& alloc = "
               "allocator_type())"
            << std::endl;
  segment_vector<TestNode> tmp(1000);
  CHECK(tmp.size() == 1000);
}

void Test_segment_vector_with_size_value() {
  std::cout << "Test segment_vector(size_type n, const _Tp& value, const "
               "allocator_type& alloc = allocator_type())"
            << std::endl;
  segment_vector<TestNode> tmp(1000, TestNode(123, "success"));
  CHECK(tmp.begin()->m_str == "success");
  CHECK(tmp.begin()->m_value == 123);
  CHECK(tmp.size() == 1000);
  CHECK((tmp.end() - 1)->m_str == "success");
}

void Test_segment_vector_with_node() {
  std::cout << "Test segment_vector(const segment_vector& other)" << std::endl;
  segment_vector<TestNode> my_tmp(1000, TestNode(123, "success"));
  segment_vector<TestNode> tmp(my_tmp);
  CHECK(tmp.begin()->m_str == "success");
  CHECK(tmp.begin()->m_value == 123);
  CHECK(tmp.size() == 1000);
  CHECK((tmp.end() - 1)->m_str == "success");
}

void Test_segment_vector_with_move() {
  std::cout << "Test segment_vector(segment_vector&& other)" << std::endl;
  segment_vector<TestNode> my_tmp(1000, TestNode(123, "success"));
  segment_vector<TestNode> tmp(std::move(my_tmp));
  CHECK(tmp.begin()->m_str == "success");
  CHECK(tmp.begin()->m_value == 123);
  CHECK(tmp.size() == 1000);
  CHECK((tmp.end() - 1)->m_str == "success");
  CHECK(my_tmp.size() == 0);
}

void Test_segment_vector_with_list() {
  std::cout << "Test segment_vector(std::initializer_list<value_type> l, const "
               "allocator_type& a = allocator_type())"
            << std::endl;
  segment_vector<TestNode> tmp({TestNode(1, "success1"),
                                TestNode(2, "success2"),
                                TestNode(3, "success3")});
  CHECK(tmp.size() == 3);
  CHECK(tmp.begin()->m_str == "success1");
  CHECK((tmp.end() - 1)->m_str == "success3");
  CHECK(tmp.begin()->m_value == 1);
  CHECK(tmp[0].m_str == "success1");
  CHECK(tmp.at(0).m_str == "success1");
  CHECK(tmp[2].m_value == 3);
}

void Test_push_back_with_value() {
  std::cout << "Test void push_back(const value_type& x) " << std::endl;
  segment_vector<TestNode> tmp(1000, TestNode(123, "success"));
  TestNode tmp_node(100, "right");
  tmp.push_back(tmp_node);
  CHECK(tmp.size() == 1001);
  CHECK(tmp[1000].m_str == "right");
  CHECK(tmp[1000].m_value == 100);
  for (int i = 0; i < 50000; ++i) {
    TestNode tmp_node(i, "right");
    tmp.push_back(tmp_node);
  }
  CHECK(tmp.size() == 51001);
  CHECK(tmp[51000].m_value == 49999);
}

void Test_push_back_with_move_value() {
  std::cout << "Test void push_back(value_type&& x)" << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 5000; ++i) {
    tmp.push_back(std::move(TestNode(i, "helloworld")));
  }
  CHECK(tmp.size() == 5000);
  CHECK(tmp[4999].m_value == 4999);
  CHECK(tmp.begin()->m_str == "helloworld");
}

void Test_emplace_back_with_value() {
  std::cout << "Test void emplace_back(Args&&... args)" << std::endl;
  segment_vector<TestNode> tmp;
  TestNode tmp_node(1, "yes");
  tmp.emplace_back(std::move(tmp_node));
  CHECK(tmp.size() == 1);
  CHECK(tmp[0].m_str == "yes");
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp[i + 1].m_value == i);
    CHECK(tmp[i + 1].m_str == "hello");
  }
  CHECK(tmp.size() == 10001);
}

void Test_emplace_with_iterator() {
  std::cout << "Test iterator emplace(const_iterator position, Args&&... args) "
               "time cost expensive"
            << std::endl;
  segment_vector<TestNode> tmp(3000, TestNode(123, "construct"));
  for (int i = 0; i < 50000; ++i) {
    TestNode tmp_node(i, "hello");
    tmp.emplace(tmp.end(), tmp_node);
    CHECK(tmp.back().m_str == "hello");
    CHECK(tmp.back().m_value == i);
  }
  // cost expensive
  for (int i = 0; i < 50000; ++i) {
    TestNode tmp_node(i, "hello");
    tmp.emplace(tmp.begin(), tmp_node);
    // std::cout << i << std::endl;
    CHECK(tmp.front().m_str == "hello");
    CHECK(tmp.front().m_value == i);
  }
  for (int i = 0; i < 5000; ++i) {
    TestNode tmp_node(i, "hello");
    tmp.emplace(tmp.begin() + i, tmp_node);
    CHECK((tmp.begin() + i)->m_str == "hello");
    CHECK((tmp.begin() + i)->m_value == i);
  }
  for (int i = 0; i < 50000; ++i) {
    TestNode tmp_node(i, "hello");
    tmp.emplace(tmp.end(), tmp_node);
    CHECK(tmp.back().m_str == "hello");
    CHECK(tmp.back().m_value == i);
  }
}

void Test_pop_back() {
  std::cout << "Test void pop_back() noexcept" << std::endl;
  segment_vector<TestNode> tmp(5000, TestNode(1, "hello"));
  for (int i = 0; i < 5000; ++i) {
    tmp.pop_back();
  }
  CHECK(tmp.size() == 0);
  CHECK(tmp.empty() == true);
}

void Test_back_front() {
  std::cout << "Test back() && front()" << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
}

void Test_reserve() {
  std::cout << "Test  void reserve(size_type n)" << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 1000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
  tmp.reserve(100000);
  CHECK(tmp.back().m_value == 999);
  CHECK(tmp.back().m_str == "hello");
  CHECK(tmp.front().m_value == 0);
  CHECK(tmp.size() == 1000);
}

void Test_resize_with_n() {
  std::cout << "Test void resize(size_type new_size, const value_type& x)"
            << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
  CHECK(tmp.size() == 10000);
  tmp.resize(100);
  CHECK(tmp.size() == 100);
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
  CHECK(tmp.size() == 10100);
  tmp.resize(50000);
  CHECK(tmp.size() == 50000);
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
  CHECK(tmp.size() == 60000);
  tmp.reserve(100000);
  tmp.resize(80000);
  CHECK(tmp.size() == 80000);
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
}

void Test_resize_with_n_value() {
  std::cout << "Test void resize(size_type new_size, const value_type& x)"
            << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
  CHECK(tmp.size() == 10000);
  tmp.resize(100, TestNode(666, "right"));
  CHECK(tmp.size() == 100);
  CHECK(tmp[1].m_value == 1);
  CHECK(tmp[1].m_str == "hello");
  CHECK(tmp[99].m_value == 99);
  CHECK(tmp[99].m_str == "hello");

  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "world"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
  CHECK(tmp.size() == 10100);
  tmp.resize(50000, TestNode(777, "success"));
  CHECK(tmp[0].m_value == 0);
  CHECK(tmp[0].m_str == "hello");
  CHECK(tmp[100].m_value == 0);
  CHECK(tmp[100].m_str == "world");
  CHECK(tmp[49999].m_str == "success");
  CHECK(tmp[49999].m_value == 777);
  CHECK(tmp[10099].m_str == "world");
  CHECK(tmp[10099].m_value == 9999);
  CHECK(tmp.size() == 50000);
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
  CHECK(tmp.size() == 60000);
  tmp.reserve(100000);
  tmp.resize(80000, TestNode(888, "left"));
  CHECK(tmp.size() == 80000);
  CHECK(tmp.back().m_value == 888);
  for (int i = 0; i < 10000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
}

void Test_shrink_to_fit() {
  std::cout << "Test void shrink_to_fit() " << std::endl;
  segment_vector<TestNode> tmp;
  tmp.reserve(10000);
  for (int i = 0; i < 1000; ++i) {
    tmp.emplace_back(TestNode(i, "hello"));
    CHECK(tmp.back().m_value == i);
    CHECK(tmp.front().m_value == 0);
  }
  std::cout << " before shrink_to_fit: " << tmp.size() << std::endl;
  tmp.shrink_to_fit();
  std::cout << "after shrink_to_fit: " << tmp.size() << std::endl;
  CHECK(tmp.back().m_str == "hello");
  CHECK(tmp.back().m_value == 999);
  CHECK(tmp.front().m_str == "hello");
  CHECK(tmp.front().m_value == 0);
}

void Test_insert_with_value() {
  std::cout
      << "Test iterator insert(const_iterator position, const value_type& x)"
      << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 5000; ++i) {
    TestNode tmp_node(i, "succeed");
    tmp.insert((tmp.begin() + i), tmp_node);
    CHECK((tmp.begin() + i)->m_value == i);
    CHECK((tmp.begin() + i)->m_str == "succeed");
  }
  tmp.resize(10000);
  for (int i = 0; i < 5000; ++i) {
    TestNode tmp_node(i, "succeed");
    tmp.insert((tmp.begin() + i), tmp_node);
    CHECK((tmp.begin() + i)->m_value == i);
    CHECK((tmp.begin() + i)->m_str == "succeed");
  }
}

void Test_insert_with_right_value() {
  std::cout << "Test iterator insert(const_iterator position, value_type&& x)"
            << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 5000; ++i) {
    tmp.insert((tmp.begin() + i), std::move(TestNode(i, "succeed")));
    CHECK((tmp.begin() + i)->m_value == i);
    CHECK((tmp.begin() + i)->m_str == "succeed");
  }
  tmp.resize(10000);
  for (int i = 0; i < 5000; ++i) {
    tmp.insert((tmp.begin() + i), std::move(TestNode(i, "succeed")));
    CHECK((tmp.begin() + i)->m_value == i);
    CHECK((tmp.begin() + i)->m_str == "succeed");
  }
}

void Test_insert_with_pos_n_value() {
  std::cout << "Test iterator insert(const_iterator position, size_type n, "
               "const value_type& x) time cost expensive"
            << std::endl;
  segment_vector<TestNode> tmp;
  tmp.reserve(1000);
  for (int i = 0; i < 5000; ++i) {
    TestNode tmp_node(i, "succeed");
    tmp.insert(tmp.end(), 2000, tmp_node);
    CHECK((tmp.end() - 1999)->m_value == i);
    CHECK((tmp.end() - 1000)->m_value == i);
    CHECK((tmp.end() - 1)->m_value == i);
  }
  for (int i = 0; i < 5000; ++i) {
    TestNode tmp_node(i, "succeed");
    // std::cout << i << std::endl;
    tmp.insert(tmp.begin(), 2000, tmp_node);
    CHECK((tmp.begin() + 1999)->m_value == i);
    CHECK((tmp.begin() + 1000)->m_value == i);
    CHECK((tmp.begin())->m_value == i);
  }
}

void Test_insert_with_initializer_list() {
  std::cout << "Test iterator insert(const_iterator position, "
               "std::initializer_list<value_type> l)"
            << std::endl;
  segment_vector<TestNode> tmp;
  tmp.reserve(2000);
  for (int i = 0; i < 5000; ++i) {
    tmp.insert(tmp.end(), {TestNode(i, "success1"), TestNode(i, "success2"),
                           TestNode(i, "success3")});
    CHECK((tmp.end() - 1)->m_str == "success3");
    CHECK((tmp.end() - 1)->m_value == i);
    CHECK((tmp.end() - 2)->m_str == "success2");
    CHECK((tmp.end() - 2)->m_value == i);
    CHECK((tmp.end() - 3)->m_str == "success1");
    CHECK((tmp.end() - 3)->m_value == i);
  }
  for (int i = 0; i < 5000; ++i) {
    // std::cout << i << std::endl;
    tmp.insert(tmp.begin(), {TestNode(i, "success1"), TestNode(i, "success2"),
                             TestNode(i, "success3")});
    CHECK((tmp.begin())->m_str == "success1");
    CHECK((tmp.begin())->m_value == i);
    CHECK((tmp.begin() + 1)->m_str == "success2");
    CHECK((tmp.begin() + 1)->m_value == i);
    CHECK((tmp.begin() + 2)->m_str == "success3");
    CHECK((tmp.begin() + 2)->m_value == i);
  }
  cout << "Test insert(const_iterator, InputIt, InputIt)." << endl;
  {
    segment_vector<int> v;

    {
      std::istringstream iss("100 101 102 103 104 105 106 107 108 109");

      auto res = v.insert(v.begin(), std::istream_iterator<int>(iss),
                          std::istream_iterator<int>());

      CHECK(res == v.begin());
      CHECK(v.size() == 10);
      CHECK(v[0] == 100);
      CHECK(v[1] == 101);
      CHECK(v[2] == 102);
      CHECK(v[3] == 103);
      CHECK(v[4] == 104);
      CHECK(v[5] == 105);
      CHECK(v[6] == 106);
      CHECK(v[7] == 107);
      CHECK(v[8] == 108);
      CHECK(v[9] == 109);
    }
    std::cout << "iss_second" << std::endl;
    {
      std::istringstream iss("200 201 202 203 204 205 206 207 208 209");
      auto res = v.insert(v.begin() + 7, std::istream_iterator<int>(iss),
                          std::istream_iterator<int>());
      CHECK(res == v.begin() + 7);
      CHECK(v.size() == 20);
      CHECK(v[0] == 100);
      CHECK(v[1] == 101);
      CHECK(v[2] == 102);
      CHECK(v[3] == 103);
      CHECK(v[4] == 104);
      CHECK(v[5] == 105);
      CHECK(v[6] == 106);
      CHECK(v[7] == 200);
      CHECK(v[8] == 201);
      CHECK(v[9] == 202);
      CHECK(v[10] == 203);
      CHECK(v[11] == 204);
      CHECK(v[12] == 205);
      CHECK(v[13] == 206);
      CHECK(v[14] == 207);
      CHECK(v[15] == 208);
      CHECK(v[16] == 209);
      CHECK(v[17] == 107);
      CHECK(v[18] == 108);
      CHECK(v[19] == 109);
    }
  }
}

void Test_erase() {
  std::cout << "Test iterator erase(const_iterator position)" << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 5000; ++i) {
    tmp.emplace_back(TestNode(i, "success"));
  }
  tmp.erase(tmp.begin());
  CHECK(tmp.front().m_value == 1);
  tmp.erase(tmp.begin() + 10);
  CHECK(tmp[500].m_value == 502);
  auto it = tmp.begin();
  for (int i = 0; i < 100; ++i) {
    it = tmp.erase(it);
  }
}

void Test_erase_two_iterator() {
  std::cout << "Test iterator erase(const_iterator first, const_iterator last)"
            << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 5000; ++i) {
    tmp.emplace_back(TestNode(i, "success"));
  }
  tmp.erase(tmp.begin(), tmp.end() - 1);
  CHECK(tmp.size() == 1);
}

void Test_clear() {
  std::cout << "Test void clear() noexcept" << std::endl;
  segment_vector<TestNode> tmp;
  for (int i = 0; i < 5000; ++i) {
    tmp.emplace_back(TestNode(i, "success"));
  }
  tmp.clear();
  CHECK(tmp.size() == 0);
}

void Test_assign_with_val() {
  std::cout << "Test void assign(size_type n, const value_type& val)"
            << std::endl;
  segment_vector<int> v;
  {

    v.assign(10000, 10);
    CHECK(v.size() == 10000);
    CHECK(v[0] == 10);
    CHECK(v[1] == 10);
    CHECK(v[2] == 10);
    CHECK(v[3] == 10);
    CHECK(v[4] == 10);
    CHECK(v[5] == 10);
    CHECK(v[6] == 10);
    CHECK(v[7] == 10);
    CHECK(v[8] == 10);
    CHECK(v[9] == 10);
    CHECK(v[9999] == 10);
  }

  {
    v.assign(5, 20);
    CHECK(v.size() == 5);
    CHECK(v[0] == 20);
    CHECK(v[1] == 20);
    CHECK(v[2] == 20);
    CHECK(v[3] == 20);
    CHECK(v[4] == 20);
  }

  {
    v.assign(9, 30);
    CHECK(v.size() == 9);
    CHECK(v[0] == 30);
    CHECK(v[1] == 30);
    CHECK(v[2] == 30);
    CHECK(v[3] == 30);
    CHECK(v[4] == 30);
    CHECK(v[5] == 30);
    CHECK(v[6] == 30);
    CHECK(v[7] == 30);
    CHECK(v[8] == 30);
  }

  {
    v.assign(0, 40);
    CHECK(v.size() == 0);
  }

  {
    v.assign(4, 50);
    CHECK(v.size() == 4);
    CHECK(v[0] == 50);
    CHECK(v[1] == 50);
    CHECK(v[2] == 50);
    CHECK(v[3] == 50);
  }
}

void Test_assign_with_iterator() {
  std::cout << "Test void assign(InputIterator first, InputIterator last)"
            << std::endl;
  cout << "Test assign(InputIt, InputIt)." << endl;
  {
    segment_vector<int> v;

    {
      std::istringstream iss("11 12 13 14 15 16 17 18 19");

      v.assign(std::istream_iterator<int>(iss), std::istream_iterator<int>());

      CHECK(v.size() == 9);
      CHECK(v[0] == 11);
      CHECK(v[1] == 12);
      CHECK(v[2] == 13);
      CHECK(v[3] == 14);
      CHECK(v[4] == 15);
      CHECK(v[5] == 16);
      CHECK(v[6] == 17);
      CHECK(v[7] == 18);
      CHECK(v[8] == 19);
    }

    {
      std::istringstream iss("21 22 23 24 25");

      v.assign(std::istream_iterator<int>(iss), std::istream_iterator<int>());
      CHECK(v.size() == 5);
      CHECK(v[0] == 21);
      CHECK(v[1] == 22);
      CHECK(v[2] == 23);
      CHECK(v[3] == 24);
      CHECK(v[4] == 25);
    }

    {
      std::istringstream iss("31 32 33 34 35 36 37");

      v.assign(std::istream_iterator<int>(iss), std::istream_iterator<int>());

      CHECK(v.size() == 7);
      CHECK(v[0] == 31);
      CHECK(v[1] == 32);
      CHECK(v[2] == 33);
      CHECK(v[3] == 34);
      CHECK(v[4] == 35);
      CHECK(v[5] == 36);
      CHECK(v[6] == 37);
    }

    {
      std::istringstream iss("");
      v.assign(std::istream_iterator<int>(iss), std::istream_iterator<int>());
      CHECK(v.size() == 0);
    }

    {
      std::istringstream iss("41 42");
      v.assign(std::istream_iterator<int>(iss), std::istream_iterator<int>());
      CHECK(v.size() == 2);
      CHECK(v[0] == 41);
      CHECK(v[1] == 42);
    }
  }

  cout << "Test assign(ForwardIt, ForwardIt)." << endl;
  {
    segment_vector<int> v;

    {
      const std::vector<int> data({11, 12, 13, 14, 15, 16, 17, 18, 19});

      v.assign(data.begin(), data.end());

      CHECK(v.size() == 9);
      CHECK(v[0] == 11);
      CHECK(v[1] == 12);
      CHECK(v[2] == 13);
      CHECK(v[3] == 14);
      CHECK(v[4] == 15);
      CHECK(v[5] == 16);
      CHECK(v[6] == 17);
      CHECK(v[7] == 18);
      CHECK(v[8] == 19);
    }

    {
      const std::vector<int> data({21, 22, 23, 24, 25});

      v.assign(data.begin(), data.end());
      CHECK(v.size() == 5);
      CHECK(v[0] == 21);
      CHECK(v[1] == 22);
      CHECK(v[2] == 23);
      CHECK(v[3] == 24);
      CHECK(v[4] == 25);
    }

    {
      const std::vector<int> data({31, 32, 33, 34, 35, 36, 37});
      v.assign(data.begin(), data.end());
      CHECK(v.size() == 7);
      CHECK(v[0] == 31);
      CHECK(v[1] == 32);
      CHECK(v[2] == 33);
      CHECK(v[3] == 34);
      CHECK(v[4] == 35);
      CHECK(v[5] == 36);
      CHECK(v[6] == 37);
    }

    {
      const std::vector<int> data;
      v.assign(data.begin(), data.end());
      CHECK(v.size() == 0);
    }

    {
      const std::vector<int> data({41, 42});

      v.assign(data.begin(), data.end());
      CHECK(v.size() == 2);
      CHECK(v[0] == 41);
      CHECK(v[1] == 42);
    }
  }
}

void Test_operator_equal_with_value() {
  std::cout << "Test operator=(const segment_vector& x)" << std::endl;
  segment_vector<int> v1({10, 20, 30});
  segment_vector<int> v2({40, 50});

  CHECK(v1.size() == 3);
  CHECK(v2.size() == 2);

  v1 = v2;

  CHECK(v1.size() == 2);
  CHECK(v1[0] == 40);
  CHECK(v1[1] == 50);

  CHECK(v2.size() == 2);
  CHECK(v2[0] == 40);
  CHECK(v2[1] == 50);

  v2.push_back(80);
  v2.push_back(100);
  v1 = v2;

  CHECK(v1.size() == 4);
  CHECK(v1[0] == 40);
  CHECK(v1[1] == 50);
  CHECK(v1[2] == 80);
  CHECK(v1[3] == 100);
}

void Test_operator_equal_with_move_value() {
  std::cout << "Test operator=(segment_vector&& x)" << std::endl;

  segment_vector<int> v1({10, 20, 30});
  segment_vector<int> v2({40, 50});

  CHECK(v1.size() == 3);
  CHECK(v2.size() == 2);

  v1 = std::move(v2);

  CHECK(v1.size() == 2);
  CHECK(v1[0] == 40);
  CHECK(v1[1] == 50);

  // v2 is in a valid but unspecified state

  v2 = std::move(v1);

  // v1 is in a valid but unspecified state

  CHECK(v2.size() == 2);
  CHECK(v2[0] == 40);
  CHECK(v2[1] == 50);
}

void Test_operator_equal_with_list() {
  std::cout << "Test operator=(std::initializer_list<value_type> l)"
            << std::endl;

  segment_vector<int> v1({10, 20, 30});
  CHECK(v1.size() == 3);

  v1 = {40, 50, 60};

  CHECK(v1.size() == 3);
  CHECK(v1[0] == 40);
  CHECK(v1[1] == 50);
  CHECK(v1[2] == 60);
}

void Test_non_member_operators() {
  std::cout << "Test non_member_operators. " << std::endl;

  segment_vector<int> v1({10, 20, 30});
  segment_vector<int> v2({10, 20, 30, 40});

  CHECK(v1 == v1);
  CHECK(!(v1 != v1));
  CHECK(!(v1 == v2));
  CHECK(v1 != v2);
  CHECK(!(v1 < v1));
  CHECK(v1 < v2);
  CHECK(v1 <= v1);
  CHECK(v1 >= v1);
  CHECK(v1 <= v2);
  CHECK(v2 > v1);
  CHECK(v2 >= v1);
  CHECK(v2 >= v2);
}

void Test_mixed_operator() {
  std::cout << "Test mixed_operators. " << std::endl;
  segment_vector<TestNode> seg_vec;
  std::vector<TestNode> vec;
  seg_vec.reserve(100);
  vec.reserve(100);
  seg_vec.resize(1000);
  vec.resize(1000);
  for (int i = 0; i < 1000; ++i) {
    seg_vec.emplace_back(TestNode(i, "hello"));
    vec.emplace_back(TestNode(i, "hello"));
  }
  for (int i = 0; i < 1000; ++i) {
    seg_vec.push_back(TestNode(i, "hello"));
    vec.push_back(TestNode(i, "hello"));
  }
  for (int i = 0; i < 1000; ++i) {
    seg_vec.emplace(seg_vec.begin(), TestNode(i, "hello"));
    vec.emplace(vec.begin(), TestNode(i, "hello"));
  }
  for (int i = 0; i < 1000; ++i) {
    seg_vec.insert(seg_vec.begin() + i, TestNode(i, "hello"));
    vec.insert(vec.begin() + i, TestNode(i, "hello"));
  }
  CHECK(seg_vec.size() == vec.size());
  seg_vec.shrink_to_fit();
  vec.shrink_to_fit();
  CHECK(seg_vec.size() == vec.size());
  seg_vec.resize(10000, TestNode(1, "world"));
  vec.resize(10000, TestNode(1, "world"));
  for (int i = 0; i < 1000; ++i) {
    seg_vec.emplace(seg_vec.begin(), TestNode(i, "hello"));
    vec.emplace(vec.begin(), TestNode(i, "hello"));
  }
  for (int i = 0; i < 1000; ++i) {
    seg_vec.insert(seg_vec.begin() + i, TestNode(i, "hello"));
    vec.insert(vec.begin() + i, TestNode(i, "hello"));
  }
  for (int i = 0; i < 1000; ++i) {
    seg_vec.pop_back();
    vec.pop_back();
  }
  CHECK(seg_vec.size() == vec.size());
  for (unsigned i = 0; i < seg_vec.size(); ++i) {
    CHECK(seg_vec[i].m_str == vec[i].m_str);
    CHECK(seg_vec[i].m_value == vec[i].m_value);
  }
}

int main() {
  // Test_segment_vector_allocator();
  // Test_segment_vector_with_size();
  // Test_segment_vector_with_size_value();
  // Test_segment_vector_with_node();
  // Test_segment_vector_with_move();
  // Test_segment_vector_with_list();
  // Test_push_back_with_value();
  // Test_push_back_with_move_value();
  // Test_emplace_back_with_value();
  // Test_pop_back();
  // Test_back_front();
  // Test_reserve();
  // Test_shrink_to_fit();
  // Test_resize_with_n();
  // Test_resize_with_n_value();

  // Test_insert_with_value();
  // Test_insert_with_right_value();

  // Test_insert_with_initializer_list();
  // Test_erase();
  // Test_erase_two_iterator();
  // Test_clear();
  // Test_assign_with_val();
  // Test_assign_with_iterator();
  // Test_operator_equal_with_value();
  // Test_operator_equal_with_move_value();
  // Test_operator_equal_with_list();
  // Test_non_member_operators();

  // time cost expensive
  // Test_insert_with_pos_n_value();
  // Test_emplace_with_iterator();
  //Test_mixed_operator();
  return 0;
}