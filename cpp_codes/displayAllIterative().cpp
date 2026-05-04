//  void displayAllIterative() const {
//      if (itemCount == 0) { cout << "Inventory is empty.\n"; return; }
//      printTableHeader();
//      for (int i = 0; i < itemCount; i++) {  // ITERATIVE
//          items[i].display(i);
//      }
//      cout << "Total: " << itemCount << " product(s)\n";
//  }
//
//  case 4 in main():
//  {
//      sectionHeader("DISPLAY INVENTORY (Iterative)");
//      cout << "Algorithm: for-loop traversal\n";
//      inv.displayAllIterative();
//      pressEnter();
//      break;
//  }