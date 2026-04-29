// ---- UNDO (Stack - LIFO) ----
    void undoLastAction() {
        if (undoStack.isEmpty()) {
            cout << YELLOW << "  [INFO] Nothing to undo.\n" << RESET;
            return;
        }
        Action a = undoStack.pop();
        if (a.type == "ADD") {
            int idx = findProduct(a.productName);
            if (idx != -1) {
                int nq = items[idx].getQuantity() - a.quantity;
                if (nq <= 0) {
                    for (int i = idx; i < itemCount - 1; i++) items[i] = items[i + 1];
                    itemCount--;
                } else {
                    items[idx].setQuantity(nq);
                }
            }
            cout << CYAN << "  [UNDO] Reversed ADD of " << a.quantity
                 << " x '" << a.productName << "'.\n" << RESET;
        } else {
            int idx = findProduct(a.productName);
            if (idx != -1) {
                items[idx].addQuantity(a.quantity);
            } else if (itemCount < MAX_ITEMS) {
                items[itemCount++] = Product(a.productName, a.quantity, a.batchDate, a.expiryDate);
            }
            cout << CYAN << "  [UNDO] Restored " << a.quantity
                 << " x '" << a.productName << "'.\n" << RESET;
        }
    }