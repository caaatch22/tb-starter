#ifndef TBS_PMR_UNIQUE_PTR_HPP
#define TBS_PMR_UNIQUE_PTR_HPP

#include <memory>
#include <memory_resource>

namespace tbs {

template <typename T>
class polymorphic_delete {
 public:
  explicit polymorphic_delete(std::pmr::memory_resource* mr) : _mr(mr) {}

  void operator()(T* p) const {
    p->~T();
    _mr->deallocate(p, sizeof(T), alignof(T));
  }

 private:
  std::pmr::memory_resource* _mr = nullptr;
};

// Type of polymorphic unique pointer
template <typename T>
using polymorphic_unique_ptr = std::unique_ptr<T, polymorphic_delete<T>>;

// Makes a unique_ptr using the given polymorphic_allocator.
template <typename T, typename... Args>
polymorphic_unique_ptr<T> make_polymorphic_unique(
    std::pmr::polymorphic_allocator<T>& alloc,
    Args&&... args) {
  T* p = alloc.allocate(1);
  alloc.construct(p, std::forward<Args>(args)...);
  return std::unique_ptr<T, polymorphic_delete<T>>(
      p, polymorphic_delete<T>(alloc.resource()));
}

// Makes a unique_ptr using a plain polymorphic memory resource.
template <class T, class... Args>
polymorphic_unique_ptr<T> make_polymorphic_unique(std::pmr::memory_resource* mr,
                                                  Args&&... args) {
  std::pmr::polymorphic_allocator<T> alloc(mr);
  T* p = alloc.allocate(1);
  alloc.construct(p, std::forward<Args>(args)...);
  return std::unique_ptr<T, polymorphic_delete<T>>(p,
                                                   polymorphic_delete<T>(mr));
}

}  // namespace tbs

#endif  // TBS_PMR_UNIQUE_PTR_HPP
