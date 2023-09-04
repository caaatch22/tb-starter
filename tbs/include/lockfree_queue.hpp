#include <atomic>
#include <memory>
#include <queue>

template <typename T>
class LockFreeQueue {
 public:
  LockFreeQueue() = default;

  void push(T value) {
    auto data = std::make_shared<T>(std::move(value));
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = nullptr;

    Node* prevTail = tail_.exchange(newNode, std::memory_order_acq_rel);
    prevTail->next = newNode;
  }

  bool tryPop(T& value) {
    Node* oldHead = head_.load(std::memory_order_relaxed);
    Node* newHead;

    do {
      if (oldHead == nullptr) {
        return false;
      }
      newHead = oldHead->next;
    } while (!head_.compare_exchange_weak(oldHead, newHead,
                                          std::memory_order_release,
                                          std::memory_order_relaxed));

    value = std::move(*newHead->data);
    delete oldHead;
    return true;
  }

  bool empty() const {
    return head_.load(std::memory_order_relaxed) == nullptr;
  }

  std::size_t size() const {
    std::size_t count = 0;
    Node* current = head_.load(std::memory_order_relaxed);
    while (current != nullptr) {
      ++count;
      current = current->next;
    }
    return count;
  }

 private:
  struct Node {
    std::shared_ptr<T> data;
    Node* next;
  };

  std::atomic<Node*> head_{nullptr};
  std::atomic<Node*> tail_{nullptr};
};

template <typename T>
class LockFreeQueue2 {
 public:
  LockFreeQueue2() = default;

  void push(T value) {
    auto data = std::make_unique<T>(std::move(value));
    Node newNode;
    newNode.data = data.get();
    newNode.next = nullptr;

    Node* prevTail = tail_.exchange(newNode, std::memory_order_acq_rel);
    prevTail->next = data.release();
  }

  bool tryPop(T& value) {
    Node oldHead = head_.load(std::memory_order_relaxed);
    Node newHead;

    do {
      if (oldHead.next == nullptr) {
        return false;
      }
      newHead = *oldHead.next;
    } while (!head_.compare_exchange_weak(oldHead, newHead,
                                          std::memory_order_release,
                                          std::memory_order_relaxed));

    value = std::move(*newHead.data);
    delete oldHead.next;
    return true;
  }

  bool empty() const {
    return head_.load(std::memory_order_relaxed).next == nullptr;
  }

  std::size_t size() const {
    std::size_t count = 0;
    Node current = head_.load(std::memory_order_relaxed);
    while (current.next != nullptr) {
      ++count;
      current = *current.next;
    }
    return count;
  }

 private:
  struct Node {
    T* data;
    std::atomic_ref<Node*> next;
  };

  std::atomic_ref<Node> head_{nullptr};
  std::atomic_ref<Node> tail_{nullptr};
};