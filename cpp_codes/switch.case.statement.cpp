switch (choice) {

        case 1: {
            sectionHeader("ADD NEW STOCK", GREEN);
            string name = getString("Product name");
            if (name.empty()) { cout << RED << "  [ERROR] Name cannot be empty.\n" << RESET; pressEnter(); break; }
            int  qty    = getInt("Quantity");
            Date batch  = getDate("Batch date");
            Date expiry = getDate("Expiry date");
            if (expiry.toDays() < batch.toDays()) {
                cout << RED << "  [ERROR] Expiry cannot be before batch date.\n" << RESET;
                pressEnter(); break;
            }
            inv.addStock(name, qty, batch, expiry);
            pressEnter();
            break;
        }

        case 2: {
            sectionHeader("SELL / PROCESS ITEM", YELLOW);
            string name = getString("Product name");
            int    qty  = getInt("Quantity to sell");
            inv.sellItem(name, qty);
            pressEnter();
            break;
        }

        case 3: {
            sectionHeader("UNDO LAST ACTION", CYAN);
            cout << DIM << "  Data structure: Stack (LIFO)\n"
                 << "  The most recent action is always reversed first.\n\n" << RESET;
            inv.undoLastAction();
            pressEnter();
            break;
        }

        case 4: {
            sectionHeader("DISPLAY INVENTORY  (Iterative)", CYAN);
            cout << DIM << "  Algorithm: for-loop traversal — visits each item one by one.\n" << RESET;
            inv.displayAllIterative();
            pressEnter();
            break;
        }

        case 5: {
            sectionHeader("DISPLAY INVENTORY  (Recursive)", MAGENTA);
            cout << DIM << "  Algorithm: recursive function — calls itself with index+1\n"
                 << "  until the base case (index == total items) is reached.\n" << RESET;
            inv.displayAllRecursive();
            pressEnter();
            break;
        }

        case 6: {
            sectionHeader("EXPIRY ALERTS", RED);
            cout << DIM << "  Data structure: Priority Queue (Min-Heap)\n"
                 << "  heapifyUp/Down are implemented recursively.\n"
                 << "  Items with the nearest expiry date appear first.\n\n" << RESET;
            inv.viewExpiryAlerts();
            pressEnter();
            break;
        }

        case 7: {
            sectionHeader("SEARCH PRODUCT", BLUE);
            string query = getString("Product name to search");
            inv.searchProduct(query);
            pressEnter();
            break;
        }

        case 8: {
            sectionHeader("SORT INVENTORY", CYAN);
            cout << "  " << GREEN << "[1]" << RESET << " Sort by expiry date (nearest first)\n";
            cout << "  " << GREEN << "[2]" << RESET << " Sort by product name (A-Z)\n\n";
            int opt = getInt("Choice", 1, 2);
            inv.sortInventory(opt);
            pressEnter();
            break;
        }

        case 9: {
            sectionHeader("DELIVERY QUEUE", BLUE);
            cout << DIM << "  Data structure: Queue (FIFO)\n"
                 << "  The first delivery added will be the first one processed.\n\n" << RESET;
            cout << "  " << BLUE << "[1]" << RESET << " Add delivery to queue\n";
            cout << "  " << BLUE << "[2]" << RESET << " View current queue\n";
            cout << "  " << BLUE << "[3]" << RESET << " Process all deliveries (FIFO)\n\n";
            int sub = getInt("Choice", 1, 3);
            if (sub == 1) {
                string name = getString("Product name");
                if (name.empty()) { cout << RED << "  [ERROR] Name cannot be empty.\n" << RESET; pressEnter(); break; }
                int  qty    = getInt("Quantity");
                Date batch  = getDate("Batch date");
                Date expiry = getDate("Expiry date");
                if (expiry.toDays() < batch.toDays()) {
                    cout << RED << "  [ERROR] Expiry cannot be before batch date.\n" << RESET;
                    pressEnter(); break;
                }
                inv.enqueueDelivery(Product(name, qty, batch, expiry));
            } else if (sub == 2) {
                cout << "\n";
                inv.viewDeliveryQueue();
            } else {
                cout << "\n";
                inv.processDeliveryQueue();
            }
            pressEnter();
            break;
        }

        case 10: {
            sectionHeader("UNDO HISTORY  (Stack)", CYAN);
            inv.viewUndoStack();
            pressEnter();
            break;
        }
        
        case 0:
            clearScreen();
            cout << GREEN << BOLD << "\n\n"
                 << "  +--------------------------------------------+\n"
                 << "  |   Thank you for using Fresh Track!         |\n"
                 << "  |   Goodbye!                                 |\n"
                 << "  +--------------------------------------------+\n\n"
                 << RESET;
            break;

        default:
            cout << RED << "\n  [ERROR] Invalid choice. Enter 0-10.\n" << RESET;
            pressEnter();
        }

    } while (choice != 0);
