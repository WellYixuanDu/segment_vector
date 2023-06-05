#include <assert.h>
#include <iostream>
#include <iterator>
#include <memory>

#ifdef STL_NO_EXCEPTIONS
#define STL_TRY if (true)
#define STL_CATCH(x) if (false)
#defome STL_RETHROW
#else
#define STL_TRY try
#define STL_CATCH(x) catch (x)
#define STL_RETHROW throw
#endif

#define STL_ASSERT(x) assert(x)

template <typename ValueType, typename Reference, typename Pointer, size_t N>
struct segment_vector_iterator {

  template <typename, typename, typename, std::size_t>
  friend class segment_vector_iterator;

  template <typename, typename, uint32_t> friend class segment_vector;

public:
  using value_type = ValueType;
  using pointer = Pointer;
  using reference = Reference;
  using difference_type = ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
  using _Self = segment_vector_iterator;
  using size_type = size_t;

  using map_pointer = ValueType **;

private:
  pointer m_cur;
  pointer m_first;
  pointer m_last;
  map_pointer m_node;

public:
  static constexpr size_t buffer_size() noexcept { return N; }

public:
  segment_vector_iterator() = default;
  segment_vector_iterator(pointer x, map_pointer y) noexcept
      : m_cur(x), m_first(*y), m_last(*y + buffer_size()), m_node(y) {}

  segment_vector_iterator(const segment_vector_iterator &other) noexcept
      : m_cur(other.m_cur), m_first(other.m_first), m_last(other.m_last),
        m_node(other.m_node) {}

  // from iterator to const_iterator
  template <typename OtherPointer, typename OtherReference,
            typename std::enable_if<std::is_convertible<
                OtherPointer, Pointer>::value>::type * = nullptr>
  segment_vector_iterator(
      const segment_vector_iterator<ValueType, OtherReference, OtherPointer, N>
          &other) noexcept
      : m_cur(other.m_cur), m_first(other.m_first), m_last(other.m_last),
        m_node(other.m_node) {}

  segment_vector_iterator &
  operator=(const segment_vector_iterator &other) noexcept {
    this->m_cur = other.m_cur;
    this->m_first = other.m_first;
    this->m_last = other.m_last;
    this->m_node = other.m_node;
    return *this;
  }
  segment_vector_iterator _M_const_cast() const noexcept {
    return segment_vector_iterator(m_cur, m_node);
  }
  reference operator*() const noexcept { return *m_cur; }

  pointer operator->() const noexcept { return m_cur; }

  _Self &operator++() noexcept {
    ++m_cur;
    if (m_cur == m_last) {
      set_node(m_node + 1);
      m_cur = m_first;
    }
    return *this;
  }

  _Self operator++(int) noexcept {
    auto temp = *this;
    ++*this;
    return temp;
  }

  _Self &operator--() noexcept {
    if (m_cur == m_first) {
      set_node(m_node - 1);
      m_cur = m_last;
    }
    --m_cur;
    return *this;
  }

  _Self operator--(int) noexcept {
    auto temp = *this;
    --*this;
    return temp;
  }

  _Self &operator+=(difference_type n) noexcept {
    const difference_type offset = n + (m_cur - m_first);
    if (offset >= 0 && offset < difference_type(buffer_size())) {
      m_cur += n;
    } else {
      const difference_type node_offset =
          offset > 0 ? offset / difference_type(buffer_size())
                     : -difference_type((-offset - 1) / buffer_size()) - 1;
      set_node(m_node + node_offset);
      m_cur = m_first + (offset - node_offset * difference_type(buffer_size()));
    }
    return *this;
  }

  _Self &operator-=(difference_type n) noexcept { return *this += -n; }

  reference operator[](difference_type n) const noexcept {
    return *(*this + n);
  }

  friend bool operator!=(const _Self &x, const _Self &y) noexcept {
    return !(x == y);
  }
  template <typename RefR, typename PtrR>
  friend bool operator!=(
      const _Self &x,
      const segment_vector_iterator<ValueType, RefR, PtrR, N> &y) noexcept {
    return !(x == y);
  }

  friend bool operator==(const _Self &x, const _Self &y) noexcept {
    return x.m_cur == y.m_cur;
  }
  template <typename RefR, typename PtrR>
  friend bool operator==(
      const _Self &x,
      const segment_vector_iterator<ValueType, RefR, PtrR, N> &y) noexcept {
    return x.m_cur == y.m_cur;
  }

  friend bool operator<(const _Self &x, const _Self &y) noexcept {
    return (x.m_node == y.m_node) ? (x.m_cur < y.m_cur) : (x.m_node < y.m_node);
  }
  template <typename RefR, typename PtrR>
  friend bool operator<(
      const _Self &x,
      const segment_vector_iterator<ValueType, RefR, PtrR, N> &y) noexcept {
    return (x.m_node == y.m_node) ? (x.m_cur < y.m_cur) : (x.m_node < y.m_node);
  }

  friend bool operator>(const _Self &x, const _Self &y) noexcept {
    return y < x;
  }
  template <typename RefR, typename PtrR>
  friend bool operator>(
      const _Self &x,
      const segment_vector_iterator<ValueType, RefR, PtrR, N> &y) noexcept {
    return y < x;
  }

  friend bool operator<=(const _Self &x, const _Self &y) noexcept {
    return !(y < x);
  }
  template <typename RefR, typename PtrR>
  friend bool operator<=(
      const _Self &x,
      const segment_vector_iterator<ValueType, RefR, PtrR, N> &y) noexcept {
    return !(y < x);
  }

  friend bool operator>=(const _Self &x, const _Self &y) noexcept {
    return !(x < y);
  }
  template <typename RefR, typename PtrR>
  friend bool operator>=(
      const _Self &x,
      const segment_vector_iterator<ValueType, RefR, PtrR, N> &y) noexcept {
    return !(x < y);
  }

  friend difference_type operator-(const _Self &x, const _Self &y) noexcept {
    return difference_type(buffer_size()) *
               (x.m_node - y.m_node - int(x.m_node != 0)) +
           (x.m_cur - x.m_first) + (y.m_last - y.m_cur);
  }
  template <typename RefR, typename PtrR>
  friend difference_type operator-(
      const _Self &x,
      const segment_vector_iterator<value_type, RefR, PtrR, N> &y) noexcept {
    return difference_type(buffer_size()) *
               (x.m_node - y.m_node - int(x.m_node != 0)) +
           (x.m_cur - x.m_first) + (y.m_last - y.m_cur);
  }

  friend _Self operator+(const _Self &x, difference_type n) noexcept {
    _Self tmp = x;
    tmp += n;
    return tmp;
  }

  friend _Self operator-(const _Self &x, difference_type n) noexcept {
    _Self tmp = x;
    tmp -= n;
    return tmp;
  }

  friend _Self operator+(difference_type n, const _Self &x) noexcept {
    return x + n;
  }

private:
  void set_node(map_pointer new_node) {
    m_node = new_node;
    m_first = *new_node;
    // buffersize
    m_last = m_first + difference_type(buffer_size());
  }
};

// blocksize 字节
template <typename _Tp, typename _Alloc = std::allocator<_Tp>,
          uint32_t _BlockSize = 4096>
class segment_vector {
public:
public:
  // 分配器类型
  using allocator_type = _Alloc;
  // using allocator_traits = std::allocator_traits<_Alloc>;

  // 对 _Alloc 重新绑定到 _Tp类型
  using _Tp_alloc_type =
      typename std::allocator_traits<_Alloc>::template rebind_alloc<_Tp>;
  using _Alloc_traits = std::allocator_traits<_Tp_alloc_type>;

  using value_type = _Tp;
  using pointer = typename _Alloc_traits::pointer;
  using const_pointer = const typename _Alloc_traits::pointer;
  using reference = value_type &;
  using const_reference = const value_type &;
  using difference_type = typename _Alloc_traits::difference_type;
  using size_type = typename _Alloc_traits::size_type;

  using map_pointer = value_type **;

private:
  static constexpr size_t buffer_size() noexcept {
    return (sizeof(_Tp) < _BlockSize ? size_t(_BlockSize) / sizeof(_Tp)
                                     : size_t(1));
  }

public:
  using iterator = segment_vector_iterator<_Tp, _Tp &, pointer, buffer_size()>;

  using const_iterator =
      segment_vector_iterator<_Tp, const _Tp &, const_pointer, buffer_size()>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using map_allocator = typename _Alloc_traits::template rebind_alloc<pointer>;
  using map_traits = std::allocator_traits<map_allocator>;

private:
  _Tp_alloc_type m_allocator;

public:
  allocator_type &get_Tp_allocator() noexcept { return m_allocator; }
  const allocator_type &get_Tp_allocator() const noexcept {
    return m_allocator;
  }

  allocator_type get_allocator() const noexcept {
    return allocator_type(get_Tp_allocator());
  }

private:
  // 起始迭代器
  iterator m_start;
  // 末尾迭代器
  iterator m_finish;
  // 指向段指针的指针
  map_pointer m_map;

  size_type m_map_size;

public:
  // 构造函数 check
  explicit segment_vector(const allocator_type &alloc = allocator_type())
      : m_allocator(_Tp_alloc_type(alloc)), m_start(), m_finish(),
        m_map(nullptr), m_map_size(0) {
    initialize_empty();
  }

  explicit segment_vector(size_type n,
                          const allocator_type &alloc = allocator_type())
      : m_allocator(_Tp_alloc_type(alloc)), m_start(), m_finish(),
        m_map(nullptr), m_map_size(0) {
    initialize_default_n(n);
  }

  segment_vector(size_type n, const _Tp &value,
                 const allocator_type &alloc = allocator_type())
      : m_allocator(_Tp_alloc_type(alloc)), m_start(), m_finish(),
        m_map(nullptr), m_map_size(0) {
    initialize_fill_n(n, value);
  }

  segment_vector(const segment_vector &other)
      : m_allocator(_Alloc_traits::select_on_container_copy_construction(
            other.get_Tp_allocator())) {
    initialize_copy(other);
  }

  segment_vector(segment_vector &&other)
      : m_allocator(std::move(other.get_Tp_allocator())) {
    initialize_move(other);
  }

  segment_vector(const segment_vector &other, const allocator_type &alloc)
      : m_allocator(_Tp_alloc_type(alloc)) {
    initialize_copy(other);
  }

  segment_vector(segment_vector &&other, const allocator_type &alloc)
      : m_allocator(_Tp_alloc_type(alloc)) {
    initialize_move(other);
  }

  segment_vector(std::initializer_list<value_type> l,
                 const allocator_type &alloc = allocator_type())
      : m_allocator(_Tp_alloc_type(alloc)) {
    initialize_range(l.begin(), l.end(), std::random_access_iterator_tag());
  }

  template <typename InputIterator,
            typename = std::_RequireInputIter<InputIterator>>
  segment_vector(InputIterator first, InputIterator last,
                 const allocator_type &alloc = allocator_type())
      : m_allocator(_Tp_alloc_type(alloc)) {
    initialize_range(first, last, std::__iterator_category(first));
  }

  ~segment_vector() {
    destroy_data(begin(), end(), get_Tp_allocator());
    destroy_storage();
  }

public:
  iterator begin() noexcept { return m_start; }
  const_iterator begin() const noexcept { return m_start; }

  iterator end() noexcept { return m_finish; }
  const_iterator end() const noexcept { return m_finish; }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  const_iterator cbegin() const noexcept { return const_iterator(m_start); }

  const_iterator cend() const noexcept { return const_iterator(m_finish); }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }

  size_type max_size() const noexcept {
    return _S_max_size(get_Tp_allocator());
  }

  reference operator[](size_type n) noexcept {
    return this->m_start[difference_type(n)];
  }
  const_reference operator[](size_type n) const noexcept {
    return this->m_start[difference_type(n)];
  }

  reference at(size_type n) {
    range_check(n);
    return (*this)[n];
  }
  const_reference at(size_type n) const {
    range_check(n);
    return (*this)[n];
  }

  reference front() noexcept {
    STL_ASSERT(!empty());
    return *begin();
  }
  const_reference front() const noexcept {
    STL_ASSERT(!empty());
    return *begin();
  }
  reference back() noexcept {
    STL_ASSERT(!empty());
    iterator tmp = end();
    --tmp;
    return *tmp;
  }
  const_reference back() const noexcept {
    STL_ASSERT(!empty());
    const_iterator tmp = end();
    --tmp;
    return *tmp;
  }

  // todo: 不提供返回原始数组
  pointer data() noexcept;
  const_pointer data() const noexcept;

  // 交换元素
  void swap(segment_vector &x) noexcept {
    STL_ASSERT(_Alloc_traits::propagate_on_container_swap::value ||
               get_Tp_allocator() == x.get_Tp_allocator());
    if (this == &x) {
      return;
    }
    using std::swap;
    if (_Alloc_traits::propagate_on_container_swap::value) {
      swap(get_Tp_allocator(), x.get_Tp_allocator());
    }
    swap((*this).m_map, x.m_map);
    swap((*this).m_map_size, x.m_map_size);
    swap((*this).m_start, x.m_start);
    swap((*this).m_finish, x.m_finish);
  }

  // 元素增删相关接口
public:
  /*
      添加元素相关接口
  */

  // 在指定迭代器 插入元素
  template <typename... Args>
  iterator emplace(const_iterator position, Args &&... args) {
    // 若是尾部，则直接 emplace_back
    if (position.m_cur == this->m_finish.m_cur) {
      emplace_back(std::forward<Args>(args)...);
      iterator tmp = this->m_finish;
      --tmp;
      return tmp;
    } else {
      return insert_aux(position._M_const_cast(), std::forward<Args>(args)...);
    }
  }
  // 在指定位置插入元素
  iterator insert(const_iterator position, const value_type &x) {
    if (position.m_cur == this->m_finish.m_cur) {
      push_back(x);
      iterator tmp = this->m_finish;
      --tmp;
      return tmp;
    }
    return insert_aux(position._M_const_cast(), x);
  }
  // 在指定位置插入元素
  iterator insert(const_iterator position, value_type &&x) {
    return emplace(position, std::move(x));
  }
  // 在指定位置插入初始化列表
  iterator insert(const_iterator position,
                  std::initializer_list<value_type> l) {
    auto offset = position - cbegin();
    range_insert_aux(position._M_const_cast(), l.begin(), l.end(),
                     std::random_access_iterator_tag());
    return begin() + offset;
  }
  // 在指定位置 插入 n 个 x元素
  iterator insert(const_iterator position, size_type n, const value_type &x) {
    difference_type offset = position - cbegin();
    fill_insert(position._M_const_cast(), n, x);
    return begin() + offset;
  }
  // 在 指定位置 插入 迭代器范围内的元素
  template <typename InputIterator,
            typename = std::_RequireInputIter<InputIterator>>
  iterator insert(const_iterator position, InputIterator first,
                  InputIterator last) {
    difference_type offset = position - cbegin();
    range_insert_aux(position._M_const_cast(), first, last,
                     std::__iterator_category(first));
    return begin() + offset;
  }

  // push_back value
  void push_back(const value_type &x) {
    if (m_finish.m_cur != m_finish.m_last - 1) {
      _Alloc_traits::construct(get_Tp_allocator(), m_finish.m_cur, x);
      ++m_finish.m_cur;
    } else {
      push_back_aux(x);
    }
  }

  // push_back
  void push_back(value_type &&x) { emplace_back(std::move(x)); }

  // emplace_back
  template <typename... Args> void emplace_back(Args &&... args) {
    if (this->m_finish.m_cur != this->m_finish.m_last - 1) {
      _Alloc_traits::construct(get_Tp_allocator(), this->m_finish.m_cur,
                               std::forward<Args>(args)...);
      ++this->m_finish.m_cur;
    } else {
      push_back_aux(std::forward<Args>(args)...);
    }
  }

  /*
      删除元素相关接口
  */

  // 删除 指定位置的元素
  iterator erase(const_iterator position) {
    return erase_aux(position._M_const_cast());
  }

  // 删除 指定范围的元素
  iterator erase(const_iterator first, const_iterator last) {
    return erase_aux(first._M_const_cast(), last._M_const_cast());
  }

  // 删除末尾元素
  void pop_back() noexcept {
    if (empty()) {
      return;
    }
    if (this->m_finish.m_cur != this->m_finish.m_first) {
      --this->m_finish.m_cur;
      _Alloc_traits::destroy(get_Tp_allocator(), this->m_finish.m_cur);
    } else {
      pop_back_aux();
    }
  }

  // 析构全部对象
  void clear() noexcept { erase_at_end(begin()); }

public:
  segment_vector &operator=(segment_vector &x) {
    if (&x != this) {
      // 用于确定是否应该传播内存分配器的状态,即新对象是否继承原对象所使用的内存分配器
      if (_Alloc_traits::propagate_on_container_copy_assignment::value) {
        if (get_Tp_allocator() != x.get_Tp_allocator()) {
          replace_map(x, x.get_allocator());
          std::__alloc_on_copy(get_Tp_allocator(), x.get_Tp_allocator());
          return *this;
        }
        std::__alloc_on_copy(get_Tp_allocator(), x.get_Tp_allocator());
      }
      const size_type len = size();
      if (len >= x.size()) {
        erase_at_end(std::copy(x.begin(), x.end(), this->m_start));
      } else {
        iterator mid = x.begin() + difference_type(len);
        std::copy(x.begin(), mid, this->m_start);
        range_insert_aux(this->m_finish, mid, x.end(),
                         std::random_access_iterator_tag());
      }
    }
    return *this;
  }
  segment_vector &operator=(segment_vector &&x) noexcept {
    assign_move(x);
    return *this;
  }
  segment_vector &operator=(std::initializer_list<value_type> l) {
    assign_aux(l.begin(), l.end(), std::random_access_iterator_tag());
    return *this;
  }

  void assign(size_type n, const value_type &val) { assign_fill(n, val); }
  template <typename InputIterator,
            typename = std::_RequireInputIter<InputIterator>>
  void assign(InputIterator first, InputIterator last) {
    assign_aux(first, last, std::__iterator_category(first));
  }
  void assign(std::initializer_list<value_type> l) {
    assign_aux(l.begin(), l.end(), std::random_access_iterator_tag());
  }

  size_type size() const noexcept { return size_type(m_finish - m_start); }

  // 改变size个数
  void resize(size_type new_size) {
    const size_type len = size();
    if (new_size > len) {
      default_append(new_size - len);
    } else if (new_size < len) {
      erase_at_end(this->m_start + difference_type(new_size));
    }
  }

  void resize(size_type new_size, const value_type &x) {
    const size_type len = size();
    if (new_size > len) {
      fill_insert(this->m_finish, new_size - len, x);
    } else if (new_size < len) {
      erase_at_end(this->m_start + difference_type(new_size));
    }
  }

  void shrink_to_fit() {
    //释放空间
    auto seg_last = m_finish.m_node + 1;
    destory_nodes(seg_last, m_start.m_node + m_map_size);
    const size_type new_map_size = seg_last - m_start.m_node;
    const size_type new_table_capacity =
        std::max(min_table_capacity(), new_map_size);

    const size_type num_elements = size();
    map_pointer new_map = allocate_map(new_table_capacity);
    map_pointer new_map_last = new_map;

    // 复制指针
    new_map_last = std::copy(m_start.m_node, seg_last, new_map);
    // 删除旧map
    deallocate_map(m_map, m_map_size);
    // 赋新值
    m_map = new_map;
    m_start.set_node(new_map);
    m_finish.set_node(new_map_last - 1);
    m_start.m_cur = m_start.m_first;
    m_finish.m_cur = m_finish.m_first + num_elements % buffer_size();

    create_nodes(m_start.m_node + new_map_size,
                 m_start.m_node + new_table_capacity);
    m_map_size = new_table_capacity;
  }

  size_type capacity() const noexcept {
    return size_type(m_map_size * buffer_size());
  }

  bool empty() const noexcept { return begin() == end(); }
  // 不改变size个数
  void reserve(size_type n) {
    if (n > max_size()) {
      std::cout << "reserve: n is bigger than max_size" << std::endl;
    }
    if (capacity() < n) {
      reallocate_map((n / buffer_size()) - m_map_size + 1);
    }
  }

  // 构造、析构相关内部函数
private:
  // 0元素默认初始化
  void initialize_empty() { allocate_map_and_nodes(0); }

  // n元素默认初始化
  void initialize_default_n(size_type n) {
    allocate_map_and_nodes(n);

    map_pointer cur;
    STL_TRY {
      for (cur = m_start.m_node; cur < m_finish.m_node; ++cur) {
        std::__uninitialized_default(*cur, *cur + buffer_size());
      }
      std::__uninitialized_default(m_finish.m_first, m_finish.m_cur);
    }
    STL_CATCH(...) {
      destroy_storage();
      STL_RETHROW;
    }
  }

  // n元素value初始化
  void initialize_fill_n(size_type n, const _Tp &value) {
    allocate_map_and_nodes(n);
    map_pointer cur;
    STL_TRY {
      for (cur = m_start.m_node; cur < m_finish.m_node; ++cur) {
        std::__uninitialized_fill_n_a(*cur, buffer_size(), value,
                                      get_Tp_allocator());
      }
      std::__uninitialized_fill_n_a(m_finish.m_first, n % buffer_size(), value,
                                    get_Tp_allocator());
    }
    STL_CATCH(...) {
      destroy_storage();
      STL_RETHROW;
    }
  }

  // 拷贝初始化
  void initialize_copy(const segment_vector &other) {
    allocate_map_and_nodes(other.size());
    STL_TRY {
      std::__uninitialized_copy_a(other.m_start, other.m_finish, this->m_start,
                                  get_Tp_allocator());
    }
    STL_CATCH(...) {
      destroy_storage();
      STL_RETHROW;
    }
  }

  // 移动初始化
  void initialize_move(segment_vector &other) {
    // 分配器相同则直接进行数据交换
    if (get_Tp_allocator() == other.get_Tp_allocator()) {
      allocate_map_and_nodes(0);
      using std::swap;
      swap(m_start, other.m_start);
      swap(m_finish, other.m_finish);
      swap(m_map, other.m_map);
      swap(m_map_size, other.m_map_size);
    }
    // 分配器不同则首先需要分配内存空间
    else {
      allocate_map_and_nodes(other.size());
      STL_TRY {
        std::__uninitialized_move_a(other.m_start, other.m_finish, m_start,
                                    get_Tp_allocator());
      }
      STL_CATCH(...) {
        destroy_storage();
        STL_RETHROW;
      }
    }
  }

  // 初始化列表初始化
  template <typename InputIterator>
  void initialize_range(InputIterator first, InputIterator last,
                        std::input_iterator_tag) {
    allocate_map_and_nodes(0);
    STL_TRY {
      for (; first != last; ++first) {
        emplace_back(*first);
      }
    }
    STL_CATCH(...) {
      clear();
      destroy_storage();
      STL_RETHROW;
    }
  }

  // 初始化列表初始化
  template <typename ForwardIterator>
  void initialize_range(ForwardIterator first, ForwardIterator last,
                        std::forward_iterator_tag) {
    const size_type n = std::distance(first, last);
    allocate_map_and_nodes(_S_check_init_len(n, get_Tp_allocator()));
    map_pointer cur_node;

    for (cur_node = this->m_start.m_node; cur_node < this->m_finish.m_node;
         ++cur_node) {
      ForwardIterator mid = first;
      std::advance(mid, buffer_size());
      std::__uninitialized_copy_a(first, mid, *cur_node, get_Tp_allocator());
      first = mid;
    }
    std::__uninitialized_copy_a(first, last, this->m_finish.m_first,
                                get_Tp_allocator());
  }

  // 分配空间资源的内部函数
private:
  /*
  内存分配释放相关
  */

  // 分配map的空间
  map_pointer allocate_map(size_t n) {
    map_allocator map_alloc = map_allocator(get_Tp_allocator());
    return map_traits::allocate(map_alloc, n);
  }
  // 释放map的空间
  void deallocate_map(map_pointer p, size_t n) noexcept {
    map_allocator map_alloc = map_allocator(get_Tp_allocator());
    map_traits::deallocate(map_alloc, p, n);
  }

  // 分配单个节点内存
  pointer allocate_node() {
    return _Alloc_traits::allocate(get_Tp_allocator(), buffer_size());
  }
  // 释放单个节点内存空间
  void deallocate_node(pointer p) noexcept {
    _Alloc_traits::deallocate(get_Tp_allocator(), p, buffer_size());
  }

  // 分配 [nstart, nfinish) 的内存空间
  void create_nodes(map_pointer nstart, map_pointer nfinish) {
    map_pointer cur;
    STL_TRY {
      for (cur = nstart; cur < nfinish; ++cur) {
        *cur = allocate_node();
      }
    }
    STL_CATCH(...) {
      destory_nodes(nstart, nfinish);
      STL_RETHROW;
    }
  }

  // 释放 [nstart, nfinish) 的内存空间
  void destory_nodes(map_pointer nstart, map_pointer nfinish) noexcept {
    for (map_pointer n = nstart; n < nfinish; ++n) {
      deallocate_node(*n);
    }
  }

  // 分配 map 和 node 空间
  void allocate_map_and_nodes(size_type num_elements) {
    if (num_elements > max_size()) {
      std::length_error("segment_vector::allocate_map_and_nodes");
    }

    const size_type num_segments = num_elements / buffer_size() + 1;
    m_map_size = std::max(num_segments, min_table_capacity());
    m_map = allocate_map(m_map_size);

    map_pointer nstart = m_map;
    map_pointer nfinish = nstart + num_segments;
    STL_TRY {
      create_nodes(nstart, nstart + m_map_size);
      m_start.set_node(nstart);
      m_finish.set_node(nfinish - 1);
      m_start.m_cur = m_start.m_first;
      m_finish.m_cur = m_finish.m_first + num_elements % buffer_size();
    }
    STL_CATCH(...) {
      deallocate_map(m_map, m_map_size);
      STL_RETHROW;
    }
  }

  //进行 map 和 node 的内存释放
  void destroy_storage() noexcept {
    // 进行段内存的释放
    destory_nodes(this->m_map, this->m_map + m_map_size);
    // 进行map内存的释放
    deallocate_map(this->m_map, m_map_size);
  }

  // 检查是否存在n个元素的空间
  void new_elements_at_back(size_type new_elements) {
    if (this->max_size() - this->size() < new_elements) {
      std::length_error("segment_vector::new_elements_at_back");
    }

    const size_type new_nodes =
        ((new_elements + buffer_size() - 1) / buffer_size());
    reserve_map_at_back(new_nodes);
  }

  // 判断是否需要在尾部扩充内存，若需要，则进行扩充
  void reserve_map_at_back(size_type nodes_to_add = 1) {
    if (nodes_to_add + 1 >
        this->m_map_size - (this->m_finish.m_node - this->m_map)) {
      reallocate_map(nodes_to_add);
    }
  }

  // 内存扩充
  void reallocate_map(size_type nodes_to_add) {

    const size_type old_num_nodes =
        this->m_finish.m_node - this->m_start.m_node + 1;
    // const size_type new_num_nodes = old_num_nodes + nodes_to_add;

    map_pointer new_nstart;
    size_type new_map_size =
        this->m_map_size + std::max(this->m_map_size, nodes_to_add) + 1;
    map_pointer new_map = this->allocate_map(new_map_size);
    new_nstart = new_map;
    std::copy(this->m_start.m_node, this->m_start.m_node + m_map_size,
              new_nstart);
    deallocate_map(this->m_map, this->m_map_size);

    this->m_map = new_map;
    this->m_start.set_node(new_nstart);
    this->m_finish.set_node(new_nstart + old_num_nodes - 1);

    create_nodes(this->m_start.m_node + m_map_size,
                 this->m_start.m_node + new_map_size);
    this->m_map_size = new_map_size;
  }

  /*
  对象构造析构相关
  */

  // 析构 [first, last] 对象
  void destroy_data_aux(iterator first, iterator last) {
    for (map_pointer node = first.m_node + 1; node < last.m_node; ++node) {
      std::_Destroy(*node, *node + buffer_size(), get_Tp_allocator());
    }
    if (first.m_node != last.m_node) {
      std::_Destroy(first.m_cur, first.m_last, get_Tp_allocator());
      std::_Destroy(last.m_first, last.m_cur, get_Tp_allocator());
    } else {
      std::_Destroy(first.m_cur, last.m_cur, get_Tp_allocator());
    }
  }
  // 析构 [first, last] 对象
  template <typename _Alloc1>
  void destroy_data(iterator first, iterator last, const _Alloc1 &) {
    destroy_data_aux(first, last);
  }
  // 析构 [first, last] 对象
  void destroy_data(iterator first, iterator last,
                    const std::allocator<_Tp> &) {
    if (!__has_trivial_destructor(value_type)) {
      destroy_data_aux(first, last);
    }
  }

  // 进行从pos开始对象析构
  void erase_at_end(iterator pos) {
    destroy_data(pos, end(), get_Tp_allocator());
    this->m_finish = pos;
  }

private:
  // 赋值相关内部函数
private:
  // 赋值 n 个 位置为val
  void assign_fill(size_type n, const value_type &val) {
    if (n > size()) {
      std::fill(begin(), end(), val);
      fill_insert(end(), n - size(), val);
    } else {
      erase_at_end(begin() + difference_type(n));
      std::fill(begin(), end(), val);
    }
  }

  template <typename InputIterator>
  void assign_aux(InputIterator first, InputIterator last,
                  std::input_iterator_tag) {
    iterator cur = begin();
    for (; first != last && cur != end(); ++cur, (void)++first) {
      *cur = *first;
    }
    if (first == last) {
      erase_at_end(cur);
    } else {
      range_insert_aux(end(), first, last, std::__iterator_category(first));
    }
  }

  template <typename ForwardIterator>
  void assign_aux(ForwardIterator first, ForwardIterator last,
                  std::forward_iterator_tag) {
    const size_type len = std::distance(first, last);
    if (len > size()) {
      ForwardIterator mid = first;
      std::advance(mid, size());
      std::copy(first, mid, begin());
      range_insert_aux(end(), mid, last, std::__iterator_category(first));
    } else {
      erase_at_end(std::copy(first, last, begin()));
    }
  }

  void assign_move(segment_vector &x) {
    if (_Alloc_traits::propagate_on_container_move_assignment::value) {
      if (get_Tp_allocator() != x.get_Tp_allocator()) {
        segment_vector tmp(x.get_Tp_allocator());
        // 进行对象的析构
        clear();
        // 释放内存
        destroy_storage();

        m_map = nullptr;

        using std::swap;
        swap(*this, tmp);
      }
      get_Tp_allocator() = std::move(x.get_Tp_allocator());
    }
    if (get_Tp_allocator() == x.get_Tp_allocator()) {
      using std::swap;
      swap(*this, x);
    } else {
      assign_aux(std::make_move_iterator(x.m_start),
                 std::make_move_iterator(x.m_finish),
                 std::random_access_iterator_tag());
    }
  }

private:
  static size_t _S_check_init_len(size_t n, const allocator_type &a) {
    if (n > _S_max_size(a)) {
      std::length_error("cannot create segment_vector larger than max_size()");
    }
    return n;
  }
  static size_type _S_max_size(const allocator_type &a) noexcept {
    const size_t diffmax = __gnu_cxx::__numeric_traits<ptrdiff_t>::__max;
    const size_t allocmax = _Alloc_traits::max_size(a);
    return (std::min)(diffmax, allocmax);
  }
  void range_check(size_type n) const {
    if (n >= this->size()) {
      throw std::out_of_range(
          "segment_vector::range_check: n: >= this->size():");
    }
  }

  // 最小segment数量
  static constexpr size_type min_table_capacity() { return 16; }

  // 返回添加n元素后的迭代器位置
  iterator reserve_elements_at_back(size_type n) {
    const size_type vacancies =
        this->m_finish.m_last - this->m_finish.m_cur - 1;
    if (n > vacancies) {
      new_elements_at_back(n - vacancies);
    }
    return this->m_finish + difference_type(n);
  }

  // 更换数据
  void swap_data(segment_vector &x) noexcept { std::swap(*this, x); }
  // 析构并清空内存后交换数据
  template <typename... Args> void replace_map(Args &&... args) {
    segment_vector newobj(std::forward<Args>(args)...);
    clear();
    destroy_storage();
    this->m_map = nullptr;
    this->m_map_size = 0;
    this->swap_data(newobj);
  }

  // 元素增删内部函数
private:
  /*
      添加元素相关内部实现
  */
  // 尾部添加一个元素且跳至下一个段
  template <typename... Args> void push_back_aux(Args &&... args) {
    if (size() == max_size()) {
      std::length_error("cannot create segment_vector larger than max_size()");
    }
    // 判断是否需要对map进行扩展，若有必要则扩展map
    reserve_map_at_back();

    STL_TRY {
      _Alloc_traits::construct(get_Tp_allocator(), m_finish.m_cur,
                               std::forward<Args>(args)...);
      this->m_finish.set_node(this->m_finish.m_node + 1);
      this->m_finish.m_cur = this->m_finish.m_first;
    }
    STL_CATCH(...) { STL_RETHROW; }
  }

  // 在指定位置插入一个元素
  template <typename... Args>
  iterator insert_aux(iterator pos, Args &&... args) {
    value_type x_copy(std::forward<Args>(args)...);
    difference_type index = pos - this->m_start;

    push_back(std::move(back()));
    iterator back1 = this->m_finish;
    --back1;
    iterator back2 = back1;
    --back2;
    pos = this->m_start + index;
    std::move_backward(pos, back2, back1);
    *pos = std::move(x_copy);
    return pos;
  }

  // 在 pos 位置 插入 从 first 到 last的n个元素
  template <typename ForwardIterator>
  void insert_aux(iterator pos, ForwardIterator first, ForwardIterator last,
                  size_type n) {
    const difference_type elems_before = pos - this->m_start;
    const size_type len = size();
    iterator new_finish = reserve_elements_at_back(n);
    iterator old_finish = this->m_finish;
    const difference_type elems_after = difference_type(len) - elems_before;
    pos = this->m_finish - elems_after;
    STL_TRY {
      if (elems_after > difference_type(n)) {
        iterator finish_n = (this->m_finish - difference_type(n));
        std::__uninitialized_move_a(finish_n, this->m_finish, this->m_finish,
                                    get_Tp_allocator());
        this->m_finish = new_finish;
        std::move_backward(pos, finish_n, old_finish);
        std::copy(first, last, pos);
      } else {
        ForwardIterator mid = first;
        std::advance(mid, elems_after);
        std::__uninitialized_copy_move(mid, last, pos, this->m_finish,
                                       this->m_finish, get_Tp_allocator());
        this->m_finish = new_finish;
        std::copy(first, mid, pos);
      }
    }
    STL_CATCH(...) { STL_RETHROW; }
  }

  // 在 pos 位置 插入 n个x元素
  void insert_aux(iterator pos, size_type n, const value_type &x) {
    // 待插入元素之前元素的个数
    const difference_type elems_before = pos - this->m_start;
    const size_type len = this->size();
    value_type x_copy = x;
    // 新的末尾元素迭代器
    iterator new_finish = reserve_elements_at_back(n);
    // 旧末尾元素
    iterator old_finish = this->m_finish;
    // 待插入元素之后的个数
    const difference_type elems_after = difference_type(len) - elems_before;
    // 插入位置
    pos = this->m_finish - elems_after;
    STL_TRY {
      if (elems_after > difference_type(n)) {
        // 移动位置
        iterator finish_n = (this->m_finish - difference_type(n));
        std::__uninitialized_move_a(finish_n, this->m_finish, this->m_finish,
                                    get_Tp_allocator());
        this->m_finish = new_finish;
        std::move_backward(pos, finish_n, old_finish);
        std::fill(pos, pos + difference_type(n), x_copy);
      } else {
        std::__uninitialized_fill_move(this->m_finish, pos + difference_type(n),
                                       x_copy, pos, this->m_finish,
                                       get_Tp_allocator());
        this->m_finish - new_finish;
        std::fill(pos, old_finish, x_copy);
      }
    }
    STL_CATCH(...) { STL_RETHROW; }
  }

  // 在pos位置插入从 first 到 last 的元素
  template <typename InputIterator>
  void range_insert_aux(iterator pos, InputIterator first, InputIterator last,
                        std::input_iterator_tag) {
    if (pos == end()) {
      for (; first != last; ++first) {
        insert(end(), *first);
      }
    } else if (first != last) {
      segment_vector tmp(first, last, get_Tp_allocator());
      insert(pos, std::make_move_iterator(tmp.begin()),
             std::make_move_iterator(tmp.end()));
    }
  }
  // 在pos位置插入从 first 到 last 的元素
  template <typename ForwardIterator>
  void range_insert_aux(iterator pos, ForwardIterator first,
                        ForwardIterator last, std::forward_iterator_tag) {
    const size_type n = std::distance(first, last);
    if (pos.m_cur == this->m_finish.m_cur) {
      iterator new_finish = reserve_elements_at_back(n);
      STL_TRY {
        std::__uninitialized_copy_a(first, last, this->m_finish,
                                    get_Tp_allocator());
        this->m_finish = new_finish;
      }
      STL_CATCH(...) { STL_RETHROW; }
    } else {
      insert_aux(pos, first, last, n);
    }
  }

  // 在 pos 位置 插入 n个x元素
  void fill_insert(iterator pos, size_type n, const value_type &x) {
    if (pos.m_cur == this->m_finish.m_cur) {
      iterator new_finish = reserve_elements_at_back(n);
      std::__uninitialized_fill_a(this->m_finish, new_finish, x,
                                  get_Tp_allocator());
      this->m_finish = new_finish;
    } else {
      insert_aux(pos, n, x);
    }
  }

  // 在尾部添加n个元素
  void default_append(size_type n) {
    if (n) {
      iterator new_finish = reserve_elements_at_back(n);
      std::__uninitialized_default_a(this->m_finish, new_finish,
                                     get_Tp_allocator());
      this->m_finish = new_finish;
    }
  }

  /*
      删除元素相关内部实现
  */

  iterator erase_aux(iterator position) {
    iterator next = position;
    ++next;
    const difference_type index = position - begin();
    if (next != end()) {
      std::move(next, end(), position);
    }
    pop_back();
    return begin() + index;
  }

  iterator erase_aux(iterator first, iterator last) {
    if (first == last) {
      return first;
    } else if (first == begin() && last == end()) {
      clear();
      return end();
    } else {
      const difference_type n = last - first;
      const difference_type elems_before = first - begin();
      if (last != end()) {
        std::move(last._M_const_cast(), end(), first._M_const_cast());
      }
      erase_at_end(end() - n);
      return begin() + elems_before;
    }
  }

  // 析构末尾元素
  void pop_back_aux() {
    this->m_finish.set_node(this->m_finish.m_node - 1);
    this->m_finish.m_cur = this->m_finish.m_last - 1;
    _Alloc_traits::destroy(get_Tp_allocator(), this->m_finish.m_cur);
  }
};

template <typename _Tp, typename _Alloc, uint32_t _BlockSize>
inline bool operator==(segment_vector<_Tp, _Alloc, _BlockSize> &x,
                       segment_vector<_Tp, _Alloc, _BlockSize> &y) {
  return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}

template <typename _Tp, typename _Alloc, uint32_t _BlockSize>
inline bool operator<(segment_vector<_Tp, _Alloc, _BlockSize> &x,
                      segment_vector<_Tp, _Alloc, _BlockSize> &y) {
  return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename _Tp, typename _Alloc, uint32_t _BlockSize>
inline bool operator!=(segment_vector<_Tp, _Alloc, _BlockSize> &x,
                       segment_vector<_Tp, _Alloc, _BlockSize> &y) {
  return !(x == y);
}

template <typename _Tp, typename _Alloc, uint32_t _BlockSize>
inline bool operator>(segment_vector<_Tp, _Alloc, _BlockSize> &x,
                      segment_vector<_Tp, _Alloc, _BlockSize> &y) {
  return y < x;
}

template <typename _Tp, typename _Alloc, uint32_t _BlockSize>
inline bool operator<=(segment_vector<_Tp, _Alloc, _BlockSize> &x,
                       segment_vector<_Tp, _Alloc, _BlockSize> &y) {
  return !(y < x);
}

template <typename _Tp, typename _Alloc, uint32_t _BlockSize>
inline bool operator>=(segment_vector<_Tp, _Alloc, _BlockSize> &x,
                       segment_vector<_Tp, _Alloc, _BlockSize> &y) {
  return !(x < y);
}

template <typename _Tp, typename _Alloc, uint32_t _BlockSize>
inline void swap(segment_vector<_Tp, _Alloc, _BlockSize> &x,
                 segment_vector<_Tp, _Alloc, _BlockSize> &y)
    _GLIBCXX_NOEXCEPT_IF(noexcept(x.swap(y))) {
  x.swap(y);
}
