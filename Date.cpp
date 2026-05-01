
struct Date {
    int day, month, year;
    Date() : day(0), month(0), year(0) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    long toDays() const { return (long)year * 365 + month * 30 + day; }
    bool operator<(const Date& o) const { return toDays() < o.toDays(); }
    string toString() const {
        ostringstream ss;
        ss << setfill('0') << setw(2) << day << "/"
           << setw(2) << month << "/" << year;
        return ss.str();
    }
    bool isValid() const {
        return (year >= 2020 && year <= 2100 &&
                month >= 1  && month <= 12   &&
                day   >= 1  && day   <= 31);
    }
};

Date today() {
    time_t t = time(nullptr);
    tm* lt   = localtime(&t);
    return Date(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

int daysUntilExpiry(const Date& expiry) {
    return (int)(expiry.toDays() - today().toDays());
}
