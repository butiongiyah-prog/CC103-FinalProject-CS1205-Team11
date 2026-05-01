class Product {
private:
    string name;
    int    quantity;
    Date   batchDate;
    Date   expiryDate;
public:
    Product() : name(""), quantity(0) {}
    Product(string n, int q, Date batch, Date expiry)
        : name(n), quantity(q), batchDate(batch), expiryDate(expiry) {}
    string getName()     const { return name; }
    int    getQuantity() const { return quantity; }
    Date   getBatch()    const { return batchDate; }
    Date   getExpiry()   const { return expiryDate; }

    void display(int index = -1) const {
        int days = daysUntilExpiry(expiryDate);
        string statusCol, statusTxt;
        if      (days < 0)  { statusCol = RED;    statusTxt = "EXPIRED      "; }
        else if (days == 0) { statusCol = RED;    statusTxt = "EXPIRES TODAY"; }
        else if (days <= 3) { statusCol = YELLOW; statusTxt = "NEAR EXPIRY  "; }
        else                { statusCol = GREEN;  statusTxt = "GOOD         "; }

        if (index >= 0)
            cout << "  " << CYAN << setw(3) << (index+1) << "." << RESET << " ";
        else
            cout << "       ";

        cout << BOLD << left << setw(22) << name << RESET
             << setw(8)  << quantity
             << DIM << setw(14) << batchDate.toString()
             << setw(14) << expiryDate.toString() << RESET
             << statusCol << BOLD << statusTxt << RESET
             << DIM << " (" << days << " day" << (abs(days)==1?"":"s") << ")"
             << RESET << "\n";
    }
};
