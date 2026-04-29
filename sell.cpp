// ---- SELL ITEM ----
    void sellItem(const string& name, int qty) {
        int idx = findProduct(name);
        if (idx == -1) {
            cout << RED << "  [ERROR] '" << name << "' not found.\n" << RESET;
            return;
        }
        if (items[idx].getQuantity() < qty) {
            cout << RED << "  [ERROR] Not enough stock. Available: "
                 << items[idx].getQuantity() << RESET << "\n";
            return;
        }
        Action a;
        a.type = "SELL"; a.productName = items[idx].getName(); a.quantity = qty;
        a.batchDate = items[idx].getBatch(); a.expiryDate = items[idx].getExpiry();
        undoStack.push(a);

        items[idx].setQuantity(items[idx].getQuantity() - qty);
        cout << GREEN << "  [OK] Sold " << qty << " of '" << name
             << "'. Remaining: " << items[idx].getQuantity() << RESET << "\n";

        if (items[idx].getQuantity() == 0) {
            cout << YELLOW << "  [INFO] '" << name << "' is out of stock and removed.\n" << RESET;
            for (int i = idx; i < itemCount - 1; i++) items[i] = items[i + 1];
            itemCount--;
        }
    }
