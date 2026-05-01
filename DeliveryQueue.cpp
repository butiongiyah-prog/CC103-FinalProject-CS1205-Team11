class DeliveryQueue {
private:
    static const int MAX = 50;
    Product data[MAX];
    int     front, rear, count;
public:
    DeliveryQueue() : front(0), rear(-1), count(0) {}
    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == MAX; }

    void enqueue(const Product& p) {
        if (isFull()) throw runtime_error("Queue full.");
        rear = (rear + 1) % MAX;
        data[rear] = p;
        count++;
    }

    Product dequeue() {
        if (isEmpty()) throw runtime_error("Queue empty.");
        Product p = data[front];
        front = (front + 1) % MAX;
        count--;
        return p;
    }

    int size() const { return count; }

    void displayAll() const {
        if (isEmpty()) { cout << YELLOW << "  Queue is empty.\n" << RESET; return; }
        cout << "  " << BOLD
             << left << setw(6) << "#"
             << setw(22) << "Product"
             << setw(8)  << "Qty"
             << setw(14) << "Batch Date"
             << "Expiry Date" << RESET << "\n";
        cout << "  " << DIM << string(64, '-') << RESET << "\n";
        int idx = front;
        for (int i = 0; i < count; i++) {
            cout << "  " << CYAN << BOLD << setw(6) << (i + 1) << RESET
                 << left << setw(22) << data[idx].getName()
                 << setw(8) << data[idx].getQuantity()
                 << DIM << setw(14) << data[idx].getBatch().toString()
                 << data[idx].getExpiry().toString() << RESET << "\n";
            idx = (idx + 1) % MAX;
        }
    }
};