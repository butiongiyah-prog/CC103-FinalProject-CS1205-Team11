//  PRIORITY QUEUE - Min-Heap - Expiry alerts
//  heapifyUp and heapifyDown are RECURSIVE
class ExpiryPriorityQueue {
private:
    static const int MAX = 200;
    Product heap[MAX];
    int     size_;

    bool higherPriority(int a, int b) const {
        return heap[a].getExpiry() < heap[b].getExpiry();
    }

    void heapifyUp(int idx) {               // RECURSIVE
        if (idx == 0) return;               // base case: reached root
        int parent = (idx - 1) / 2;
        if (higherPriority(idx, parent)) {
            swap(heap[idx], heap[parent]);
            heapifyUp(parent);              // recursive call
        }
    }

    void heapifyDown(int idx, int sz) {     // RECURSIVE
        int left = 2*idx+1, right = 2*idx+2, smallest = idx;
        if (left  < sz && higherPriority(left,  smallest)) smallest = left;
        if (right < sz && higherPriority(right, smallest)) smallest = right;
        if (smallest != idx) {
            swap(heap[idx], heap[smallest]);
            heapifyDown(smallest, sz);      // recursive call
        }                                   // base case: smallest == idx
    }

public:
    ExpiryPriorityQueue() : size_(0) {}
    bool isEmpty() const { return size_ == 0; }
    int  size()    const { return size_; }

    void insert(const Product& p) {
        if (size_ == MAX) throw runtime_error("Priority queue full.");
        heap[size_] = p;
        heapifyUp(size_);
        size_++;
    }

    Product extractMin() {
        if (isEmpty()) throw runtime_error("Priority queue empty.");
        Product top = heap[0];
        heap[0] = heap[--size_];
        heapifyDown(0, size_);
        return top;
    }
};