//  STACK - LIFO - Undo mechanism
class InventoryStack {
private:
    static const int MAX = 100;
    Action data[MAX];
    int    top;
public:
    InventoryStack() : top(-1) {}
    bool isEmpty() const { return top == -1; }
    bool isFull()  const { return top == MAX - 1; }

    void push(const Action& a) {
        if (isFull()) {
            for (int i = 0; i < MAX - 1; i++) data[i] = data[i + 1];
            data[MAX - 1] = a;
        } else {
            data[++top] = a;
        }
    }

    Action pop() {
        if (isEmpty()) throw runtime_error("Undo stack is empty.");
        return data[top--];
    }

    int size() const { return top + 1; }

    void displayAll() const {
        if (isEmpty()) { cout << YELLOW << "  No actions recorded.\n" << RESET; return; }
        cout << "  " << DIM << "Top of stack (most recent) is shown first:\n\n" << RESET;
        for (int i = top; i >= 0; i--) {
            string col = (data[
                i].type == "ADD") ? GREEN : YELLOW;
            cout << "  " << DIM << "[" << (top - i + 1) << "] " << RESET
                 << col << BOLD << setw(6) << data[i].type << RESET
                 << "  " << left << setw(22) << data[i].productName
                 << "  qty: " << BOLD << data[i].quantity << RESET << "\n";
        }
    }
};
