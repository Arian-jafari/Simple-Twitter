#include <iostream>
#include <vector>

class User {
private:
    std::string name;
    std::string family;
    std::string username;
    std::string email;
    std::string password;
    std::string birth;
    std::string gender;
    std::vector<std::string> MyTwitts;
    std::vector<std::string> Followers;

public:
    User(std::string _name, std::string _family) : name{_name}, family{_family} {}
    User() {}
    std::string get_name() const{
        return name;
    }
    std::string get_family() const{
        return family;
    }
    std::string get_username() const{
        return username;
    }
    std::string get_email() const{
        return email;
    }
    std::string get_password() const{
        return password;
    }

    std::string get_birth() const{
        return birth;
    }
    std::string get_gender() const{
        return gender;
    }
    
    void set_name(std::string newName) {
        name = newName;
    }

    void set_family(std::string newFamily) {
        family = newFamily;
    }

    void set_username(std::string newUsername) {
        username = newUsername;
    }
    void set_email(std::string newEmail) {
        email = newEmail;
    }
    void set_password(std::string newPassword) {
        password = newPassword;
    }
    void set_birth(std::string newBirth) {
        birth = newBirth;
    }
    void set_gender(std::string newGender) {
        gender = newGender;
    }
    
    void add_my_twitt(std::string twitt) {
        MyTwitts.push_back(twitt);
    }

    void add_follower(std::string username) {
        Followers.push_back(username);
    }

    std::vector<std::string>& get_twitts() {
        return MyTwitts;
    }
    std::vector<std::string>& get_followers() {
        return Followers;
    }
};
