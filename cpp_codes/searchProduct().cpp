//  void searchProduct(const string& query) const {
//      string qLow = query;
//      transform(qLow.begin(), qLow.end(), qLow.begin(), ::tolower);
//      bool found = false;
//      for (int i = 0; i < itemCount; i++) {
//          string n = items[i].getName();
//          transform(n.begin(), n.end(), n.begin(), ::tolower);
//          if (n.find(qLow) != string::npos) {
//              items[i].display(i);
//              found = true;
//          }
//      }
//      if (!found) cout << "No product matching '" << query << "'\n";
//  }
//
//  case 7 in main():
//  {
//      sectionHeader("SEARCH PRODUCT");
//      string query = getString("Product name to search");
//      inv.searchProduct(query);
//      pressEnter();
//      break;
//  }