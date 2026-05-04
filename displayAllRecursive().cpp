void displayAllRecursive() const {

  if (itemCount == 0) { cout << "Inventory is empty.\n"; return; }
      printTableHeader();
      displayRecursiveHelper(0);   // start recursion at 0
      cout << "Total: " << itemCount << " product(s)\n";
  }

  void displayRecursiveHelper(int idx) const {
      if (idx == itemCount) return;        // BASE CASE
      items[idx].display(idx);             // print current
      displayRecursiveHelper(idx + 1);     // RECURSIVE CALL
  }

  case 5 in main():
  {
      sectionHeader("DISPLAY INVENTORY (Recursive)");
      cout << "Calls itself with index+1 until base case.\n";
      inv.displayAllRecursive();
      pressEnter();
      break;
  }
