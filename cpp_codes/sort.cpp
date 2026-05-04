// ---- SORT (bubble sort, iterative) ----
    void sortInventory(int option) {
        if (itemCount == 0) { cout << YELLOW << "  Inventory is empty.\n" << RESET; return; }
        for (int i = 0; i < itemCount - 1; i++) {
            for (int j = 0; j < itemCount - i - 1; j++) {
                bool sw = (option == 1)
                    ? items[j+1].getExpiry() < items[j].getExpiry()
                    : items[j].getName() > items[j+1].getName();
                if (sw) swap(items[j], items[j+1]);
            }
        }
        cout << GREEN << "  [OK] Sorted by "
             << (option == 1 ? "expiry date" : "product name") << ".\n\n" << RESET;
        displayAllIterative();
    }