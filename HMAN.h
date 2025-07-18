class Rental {
public:
    string renterName;
    string phoneModel;
    string phoneVariant;
    string startDate;
    string endDate;
    double totalAmount;

    bool isAlphabetic(const string& str) {
        for (char c : str) {
            if (!isalpha(c) && c != ' ') return false;
        }
        return true;
    }

    bool isNumeric(const string& str) {
        bool dotSeen = false;
        for (char c : str) {
            if (!isdigit(c)) {
                if (c == '.' && !dotSeen) dotSeen = true;
                else return false;
            }
        }
        return !str.empty();
    }

    void input();
    void display() const;
    void writeToFile(ofstream &out) const;
    bool readFromFile(ifstream &in);
};
