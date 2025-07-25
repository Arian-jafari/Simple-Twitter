#include <iostream>

class Twitt {
public:
    Twitt() {}

    void set_value(std::string val) {
        value = val;
    }

    std::string get_value() const {
        return value;
    }

    void set_id(std::string val) {
        id = val;
    }

    std::string get_id() const {
        return id;
    }

    void like() {
        like_count++;
    }

    unsigned get_likes() const {
        return like_count;
    }

    void set_like_count(unsigned number) {
        like_count = number;
    }

private:
    std::string value;
    std::string id;
    unsigned like_count{0};
};