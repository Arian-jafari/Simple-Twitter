#include "../includes/CubbyMenu.hpp"
#include "../includes/Twitter.h"
#include "../includes/Twitt.h"
#include "../includes/User.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <algorithm>

namespace fs = std::filesystem;

std::vector<User *> UserList;
std::vector<Twitt *> Twitts;

std::string username_or_email, name, family, username, email, password, birth, gender;

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void main_menu()
{
    CubbyMenu::Menu Home;
    Home.add_header("Home");
    Home.add_item("Twitts", &show_twitts);
    Home.add_item("Settings", &settings);
    Home.add_item("Add twitt", &add_twitt);
    Home.add_item("Search", &search);
    Home.add_item("Profile", []()
                  { profile(username); });
    Home.add_item("Close", &close);
    Home.print();
}

void memory_save() {
    std::fstream usernames("../data/usernames.txt");
    std::fstream emails("../data/emails.txt");
    std::fstream Twitt_like("../data/Twitt_Like.txt");
    for (auto user : UserList)
    {
        usernames << user->get_username() << std::endl;
        emails << user->get_email() << std::endl;
        std::ofstream user_twitts("../data/" + user->get_username() + "/twitts.txt");
        std::fstream user_followers("../data/" + user->get_username() + "/followers.txt");
        std::fstream user_info("../data/" + user->get_username() + "/user_info.txt");

        for (auto twt : user->get_twitts())
            user_twitts << twt << std::endl;

        for (auto flwr : user->get_followers())
            user_followers << flwr << std::endl;

        user_twitts.close();
        user_followers.close();

        user_info << user->get_name() << std::endl;
        user_info << user->get_family() << std::endl;
        user_info << user->get_email() << std::endl;
        user_info << user->get_password() << std::endl;
        user_info.close();
    }
    for (auto twitt : Twitts)
        Twitt_like << twitt->get_id() << "*" << std::to_string(twitt->get_likes()) << std::endl;

    usernames.close();
    emails.close();
    Twitt_like.close();
}
void sign_in()
{
    clear_screen();

    std::cout << "username or email: ";
    std::cin >> username_or_email;
    std::cout << "enter your password: ";
    std::cin >> password;

    bool correct_username_or_email;
    static int mistakes{0};
    for (auto user : UserList)
        if ((username_or_email == user->get_username() || username_or_email == user->get_email()) && password == user->get_password())
        {
            correct_username_or_email = true;
            std::cout << "Login successfull" << std::endl;
            name = user->get_name();
            family = user->get_family();
            username = user->get_username();
            email = user->get_email();
            birth = user->get_birth();
            gender = user->get_gender();
            password = user->get_password();
            pause();
            clear_screen();
            main_menu();
        }

    if (!correct_username_or_email)
    {
        std::cout << "Username/Email or Password is incorrect. Try again." << std::endl;
        pause();
        mistakes++;
        if (mistakes < 5)
        {
            clear_screen();
            sign_in();
            pause();
        }
        else
        {
            std::cout << "Suspicious activity detected. Try again later." << std::endl;
            pause();
            close();
        }
    }
}

void sign_up()
{
    clear_screen();

    static int mistakes{0};
    std::cout << "name: ";
    std::cin >> name;

    std::cout << "surname: ";
    std::cin >> family;

    std::cout << "username: ";
    std::cin >> username;
    bool correct_username{true};

    for (auto user : UserList)
    {
        if (username == user->get_username())
        {
            correct_username = false;
            break;
        }
    }

    bool correct_email{false};

    std::cout << "email: ";
    std::cin >> email;

    for (int i = 0; i < email.size(); i++)
        if (email[i] == '@' && i > 0 && i < email.size() - 1)
            for (int j = i + 1; j < email.size(); j++)
                if (email[j] == '.' && j < email.size() - 1)
                    correct_email = true;

    for (auto user : UserList)
    {
        if (email == user->get_email())
        {
            correct_email = false;
            break;
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "birth (yyyy/mm/dd): ";
    std::getline(std::cin, birth);

    std::cout << "gender(optional): ";
    std::getline(std::cin, gender);

    bool correct_password{false};
    std::cout << "password: ";
    std::cin >> password;

    bool Capital_Char{false}, small_char{false}, num_char{false}, special_char{false};
    for (char ch : password)
    {
        if (ch >= 'A' && ch <= 'Z')
            Capital_Char = true;
        else if (ch >= 'a' && ch <= 'z')
            small_char = true;
        else if (ch - '0' >= 0 && ch - '0' <= 9)
            num_char = true;
    }
    if ((password.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos))
        special_char = true;

    if (Capital_Char && small_char && num_char && special_char)
        correct_password = true;

    if (password.size() < 8)
        correct_password = false;

    if (!correct_email || !correct_password || !correct_username)
    {
        if (!correct_username)
        {
            std::cout << "Someone else has taken this username please choose another one." << std::endl;
        }
        else if (!correct_email)
        {
            std::cout << "This email is already taken or your email is not standard." << std::endl;
        }
        else if (!correct_password)
        {
            std::cout << "Your password must have at least 8 characters and should contain small characters, capital characters and a speacial character." << std::endl;
        }
        mistakes++;
        if (mistakes < 5)
        {
            pause();
            sign_up();
        }
        else
        {
            std::cout << "Try again later." << std::endl;
            pause();
            close();
        }
    }
    else
    {
        std::string password2;
        std::cout << "Please enter your password again : ";
        std::cin >> password2;
        if (password2 == password)
        {
            User *user = new User(name, family);
            user->set_username(username);
            user->set_email(email);
            user->set_password(password);
            UserList.push_back(user);
            std::string dir_path = "../data/" + user->get_username();
            fs::create_directories(dir_path);
            std::ofstream followers_("../data/" + user->get_username() + "/followers.txt");
            followers_.close();
            std::ofstream twitts_("../data/" + user->get_username() + "/twitts.txt");
            twitts_.close();
            std::ofstream user_info("../data/" + user->get_username() + "/user_info.txt");
            user_info.close();

            std::cout << "Sign up successful" << std::endl;
            pause();
            clear_screen();
            main_menu();
        }
        else
        {
            if (mistakes < 5)
            {
                std::cout << "Incorrect password. Try again." << std::endl;
                sign_up();
                pause();
            }
            else
            {
                std::cout << "Try agian later." << std::endl;
                pause();
                close();
            }
        }
    }
}

void show_twitts()
{
    clear_screen();
    int twitt_count{0};
    std::cout << "Twitts" << std::endl;
    for (auto user : UserList)
    {
        if (user->get_twitts().size() != 0 && (std::find(user->get_followers().begin(), user->get_followers().end(), username) != user->get_followers().end()))
        {
            twitt_count++;
            std::cout << "~" << user->get_username() << std::endl;
            for (int i = 0; i < user->get_twitts().size(); i++)
            {
                if (user->get_twitts()[i] != "")
                {
                    std::cout << user->get_twitts()[i] << std::endl;
                    std::cout << "*-*" << std::endl;
                }
            }
        }
    }
    std::string page_id;
    std::string wants = "";

    if (twitt_count == 0)
    {
        std::cout << "No twitts from your followings yet!" << std::endl;
    }
    else
    {
        std::cout << "If you want to see the page for a specific twitt enter yes : ";
        std::cin >> wants;
    }

    if (wants == "yes")
    {
        std::cout << "Enter twitt id to see it's content : ";
        std::cin >> page_id;

        bool valid_id;
        for (auto twitt : Twitts)
        {
            if (twitt->get_id() == page_id)
            {
                valid_id = true;
                break;
            }
        }
        if (valid_id)
        {
            std::string user_page;
            int j{0};
            while (page_id[j] != '-')
            {
                user_page += page_id[j];
                j++;
            }

            for (auto user : UserList)
            {
                if (user->get_username() == user_page)
                {
                    if ((std::find(user->get_followers().begin(), user->get_followers().end(), username) != user->get_followers().end()))
                    {
                        display_twitt(page_id);
                        break;
                    }
                    else
                    {
                        std::cout << "You don't follow the person who posted this twitt." << std::endl;
                        break;
                    }
                }
            }
        }
        else
        {
            std::cout << "Invalid twitt id." << std::endl;
        }
    }

    CubbyMenu::Menu Back;
    Back.add_item("Back", []()
                  { clear_screen();
                    main_menu(); });
    Back.print();
}

void settings()
{
    clear_screen();
    CubbyMenu::Menu setting;
    setting.add_header("Settings");

    setting.add_item("Change Information", &change_information);

    setting.add_item("Log out", []()
                     {
        memory_save();
        CubbyMenu::Menu menu;
        menu.add_header("Login");
        menu.add_item("Sign up", &sign_up);
        menu.add_item("Sign in", &sign_in);
        menu.add_item("Close", &close);
        clear_screen();
        menu.print(); });

    setting.add_item("Back", []()
                     {
        clear_screen();
        main_menu(); });
    setting.print();
}

void add_twitt()
{
    clear_screen();
    CubbyMenu::Menu Create;
    Create.add_header("Create");

    Create.add_item("Post a twitt", []()
                    {
        std::string your_twitt;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, your_twitt);
        for (auto user : UserList) {
            if (user->get_username() == username) {
                Twitt* _twitt = new Twitt;
                _twitt->set_value(your_twitt);
                _twitt->set_id(username + "-" + std::to_string(user->get_twitts().size() + 1));
                user->add_my_twitt(your_twitt);
                Twitts.push_back(_twitt);
                std::cout << "Your twitt posted." << std::endl;
                pause();
                add_twitt();
            }
        } });

    Create.add_item("Explore", []()
                    {
        CubbyMenu::Menu explore_twitts;
        clear_screen();
        explore_twitts.add_item("Search for a twitt", []() {
            clear_screen();
            std::string id;
            std::cout << "Twitt id : ";
            std::cin >> id;
            bool valid_id{false};
            bool flaq;
            
            for (int i = 0; i < Twitts.size(); i++) { 
                if (Twitts[i]->get_id() == id) {
                    valid_id = true;
                    for (auto user : UserList) {
                        for (int j = 0; j < user->get_twitts().size(); j++) {
                            if (user->get_twitts()[j] == Twitts[i]->get_value()) {
                                std::cout << user->get_name() << " " << user->get_family() << std::endl;
                                std::cout << "  " << Twitts[i]->get_value() << std::endl;
                                flaq = true;
                                break;
                            }
                        }
                        if (flaq == true)
                            break;                  
                    }

                    CubbyMenu::Menu reply;
                    reply.add_item("Reply to this twitt", [i, id]() {
                        std::string comment;
                        clear_screen();
                        std::cout << "Reply to " << Twitts[i]->get_id() << " : ";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::getline(std::cin, comment);

                        Twitts[i]->set_value(Twitts[i]->get_value() + "\t~" + username + " -> " + comment);
                        for (int z = 0; z < UserList.size(); z++) {
                            if (UserList[z]->get_username() == Twitts[i]->get_id().substr(0, UserList[z]->get_username().size())) {
                                UserList[z]->get_twitts()[std::stoi(id.substr(UserList[z]->get_username().size() + 1)) - 1] = Twitts[i]->get_value();
                            }
                        }
                        std::cout << "Your reply posted." << std::endl;
                    });
                    reply.add_item("Back", &add_twitt);
                    reply.print();           
                }
            }
            if (!valid_id) {
                std::cout << "Invalid twitt id." << std::endl;
            }
            CubbyMenu::Menu Back;
            Back.add_item("Back", &add_twitt);
            Back.print();
            
        });
        explore_twitts.add_item("Back", &add_twitt);
        explore_twitts.print(); });

    Create.add_item("Back", []()
                    {
        clear_screen();
        main_menu(); });
    clear_screen();
    Create.print();
}

void search()
{
    clear_screen();
    CubbyMenu::Menu srch;
    srch.add_header("Search");
    srch.add_item("Serach by username", []()
                  {
        std::cout << "Enter username : " << std::endl;
        std::string wanted_username;
        std::cin >> wanted_username;
        bool valid_username;
        for (auto user : UserList) {
            if (user->get_username() == wanted_username) {
                valid_username = true;
                break;
            }
        }
        if (wanted_username != username) {
            for (auto user : UserList) {
                if (user->get_username() == wanted_username) {
                    if ((std::find(user->get_followers().begin(), user->get_followers().end(), username) == user->get_followers().end())) {
                        std::string wants_to_follow;
                        std::cout << "Not following " << user->get_username() << "\nEnter yes to follow : ";
                        std::cin >> wants_to_follow;
                        if (wants_to_follow == "yes") {
                            user->add_follower(username);
                            std::cout << "You follow " << user->get_username() << std::endl;
                        }
                        break;
                    }
                    else {
                        std::cout << "You follow this account." << std::endl;
                        break;
                    }
                }
            }    
        }
        if (valid_username) {
            profile(wanted_username);
            pause();
        }
        
        else {
            std::cout << "There is no account with this username." << std::endl;
            CubbyMenu::Menu Back;
            Back.add_item("Back", []() {
                clear_screen();
                main_menu();
            });
            Back.print();
        } });

    srch.add_item("Search by full name", []()
                  {
        std::string _name, _family;
        std::cout << "Enter name and surname(please split them with a space between) : ";
        std::cin >> _name >> _family;

        int count_with_same_name{0};
        for (auto user : UserList) {
            if (user->get_name() == _name && user->get_family() == _family) {
                count_with_same_name++;
            }
        }
        if (count_with_same_name == 0) {
            std::cout << "No user with this information!" << std::endl;
        }
        else if (count_with_same_name == 1) {
            for (auto user : UserList) {
                if (user->get_name() == _name && user->get_family() == _family) {
                    if ((std::find(user->get_followers().begin(), user->get_followers().end(), username) == user->get_followers().end()) && (_name != name || _family != family) ) {
                        std::string wants_to_follow;
                        std::cout << "Not following " << user->get_username() <<"\nenter yes to follow this account: ";
                        std::cin >> wants_to_follow;
                        if (wants_to_follow == "yes") {
                            user->add_follower(username);
                            std::cout << "You follow " << user->get_username() << std::endl;
                            pause();
                        }
                    }

                    else {
                        if (_name != name || _family != family) {
                            std::cout << "You follow this account." << std::endl;
                            pause();
                        }
                    }
                    profile(user->get_username());
                }
            }
        }
        else {
            std::cout << "There are several users with this information we don't know who you mean please try it with username." << std::endl;
        }
        CubbyMenu::Menu Back;
        Back.add_item("Back", &search);
        Back.print(); });

    srch.add_item("Back", []()
                  {
        clear_screen();
        main_menu(); });
    srch.print();
}

void profile(std::string userId)
{
    clear_screen();

    for (auto user : UserList)
    {
        if (user->get_username() == userId)
        {

            std::cout << "Profile" << std::endl;
            std::cout << "name : " << user->get_name() << '\n'
                      << "surname : " << user->get_family() << std::endl;
            std::cout << "username : " << user->get_username() << std::endl;

            std::cout << "twitts: " << std::endl;
            for (auto twt : user->get_twitts())
            {
                if (twt != "")
                    std::cout << twt << std::endl
                              << "-*-" << std::endl;
            }
            std::cout << "----------------------------------------------------------------------------------" << std::endl;

            std::cout << "followers: " << std::endl;
            for (auto follower : user->get_followers())
            {
                std::cout << follower << std::endl;
            }
            std::cout << "----------------------------------------------------------------------------------" << std::endl;

            std::cout << "followings: " << std::endl;
            for (auto user : UserList)
            {
                if (std::find(user->get_followers().begin(), user->get_followers().end(), userId) != user->get_followers().end())
                {
                    std::cout << user->get_username() << std::endl;
                }
            }
            std::cout << "----------------------------------------------------------------------------------" << std::endl;
            std::string wants;
            if (username == userId)
                std::cout << "If you want to see more content about any specific twitt you posted please enter yes : ";
            else
                std::cout << "To see a specific twitt this user posted enter yes : ";

            std::cin >> wants;

            if (wants == "yes")
            {

                int twt_index;
                std::cout << "Index of twitt(index is specified by the time of posting twitt) : ";
                std::cin >> twt_index;

                if (user->get_twitts().size() >= twt_index && user->get_twitts()[twt_index - 1] != "")
                    display_twitt(userId + "-" + std::to_string(twt_index));
                else
                    std::cout << "No twitt with this id" << std::endl;
            }

            CubbyMenu::Menu Back;
            Back.add_item("Back", []()
                          {
                clear_screen();
                main_menu(); });
            Back.print();
        }
    }
}

void change_information()
{
    clear_screen();
    CubbyMenu::Menu change;
    change.add_header("Information Settings");
    change.add_item("Change Name", []()
                    {
                        std::string newName;
                        std::cout << "new name : ";
                        std::cin >> newName;
                        if (newName != name)
                        {
                            name = newName;
                            std::cout << "Your name changed successfully." << std::endl;
                        }
                        else
                        {
                            std::cout << "You already use this name." << std::endl;
                        }
                        CubbyMenu::Menu Back;
                        Back.add_item("Back", &change_information);
                        Back.print(); });
    change.add_item("Change Family", []()
                    {
                        std::string newFamily;
                        std::cout << "new surname : ";
                        std::cin >> newFamily;
                        if (newFamily != family)
                        {
                            family = newFamily;
                            std::cout << "Your surname changed successfully." << std::endl;
                        }
                        else
                        {
                            std::cout << "You already use this surname." << std::endl;
                        }

                        CubbyMenu::Menu Back;
                        Back.add_item("Back", &change_information);
                        Back.print(); });
    change.add_item("Change Birth", []()
                    {
        std::string newBirth;
        std::cout << "new birth : ";
        std::cin >> newBirth;
        if (newBirth != birth) {
            birth = newBirth;
            std::cout << "Your birth changed successfully." << std::endl;
        }
        else {
            std::cout << "You already have this birth date." << std::endl;
        }
        CubbyMenu::Menu Back;
        Back.add_item("Back", &change_information);
        Back.print(); });
    change.add_item("Change Gender", []()
                    {
        std::string newGender;
        std::cout << "new gender : ";
        std::cin >> newGender;
        if (newGender != gender) {
            gender = newGender;
            std::cout << "Yourgetch gender changed." << std::endl;
        }
        else {
            std::cout << "You're gender is already " << gender << "." << std::endl;
        }
        CubbyMenu::Menu Back;
        Back.add_item("Back", &change_information);
        Back.print(); });
    change.add_item("Change Password", []()
                    {
        std::string oldPassword;
        std::string newPassword;
        std::cout << "Enter your password : ";
        std::cin >> oldPassword;
        bool correct_password0;
        if (oldPassword == password) {
            std::cout << "Enter your new password : ";
            std::cin >> newPassword;
            bool Capital_Char0{false}, small_char0{false}, num_char0{false}, special_char0{false};
            for (char ch : newPassword) {
                if (ch >= 'A' && ch <= 'Z')
                    Capital_Char0 = true;
                else if (ch >= 'a' && ch <= 'z')
                    small_char0 = true;
                else if (ch - '0' >= 0 && ch - '0' <= 9)
                    num_char0 = true;
            }
            if ((newPassword.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos))
                special_char0 = true;

            if (Capital_Char0 && small_char0 && num_char0 && special_char0)
                correct_password0 = true;
            
            if (correct_password0) {
                password = newPassword;
                std::cout << "Your password changed successfully." << std::endl;
            }
            else {
                std::cout << "Your password is not standard. Try again." << std::endl;
            }
        }
        else {
            std::cout << "Password is wrong. Try again." << std::endl;
        }
        

        CubbyMenu::Menu Back;
        Back.add_item("Back", &change_information);
        Back.print(); });
    for (int i = 0; i < UserList.size(); i++)
    {
        if (UserList.at(i)->get_username() == username)
        {
            UserList.at(i)->set_name(name);
            UserList.at(i)->set_family(family);
            UserList.at(i)->set_password(password);
            UserList.at(i)->set_birth(birth);
            UserList.at(i)->set_gender(gender);
            break;
        }
    }
    change.add_item("Back", &settings);
    change.print();
}

void display_twitt(std::string t_id)
{
    clear_screen();
    for (auto twt : Twitts)
    {
        if (twt->get_id() == t_id)
        {
            std::cout << t_id << " :: ";
            std::cout << twt->get_value() << std::endl;
            std::cout << "liked by " << twt->get_likes() << std::endl;
        }
    }

    std::string user_id;
    int j{0};
    while (t_id[j] != '-')
    {
        user_id += t_id[j];
        j++;
    }

    CubbyMenu::Menu tw_display;
    tw_display.add_header(t_id);
    tw_display.add_item("like", [t_id]()
                        {
        for (int z = 0; z < Twitts.size(); z++) {
            if (Twitts[z]->get_id() == t_id) {
                Twitts[z]->like();
                std::cout << "Post liked." << std::endl;
                break;
            }
        }
        CubbyMenu::Menu Back;
        Back.add_item("Back", []() {
            clear_screen();
            main_menu();
        }); });
    tw_display.add_item("delete", [user_id, t_id]()
                        {
        if (user_id !=  username) {
            std::cout << "Someone else has posted this twitt you can't delete it." << std::endl;
        }
        else {
            int tw_index_user;       
            for (int  i = 0; i < Twitts.size(); i++) { 
                if (t_id == Twitts.at(i)->get_id()) {
                    tw_index_user = std::stoi(Twitts[i]->get_id().substr(user_id.size() + 1));
                    Twitts.erase(Twitts.begin() + i);
                    for (int j = 0; j < UserList.size(); j++) 
                        if (UserList[j]->get_username() == username) { 
                            UserList[j]->get_twitts()[tw_index_user - 1] = ""; 
                            std::cout << "Twitt deleted." << std::endl;
                            break;
                        }
                    break;
                }
            }
        }
        CubbyMenu::Menu Back;
        Back.add_item("Back", []() {
            clear_screen();
            main_menu();
        }); });

    tw_display.add_item("Back", []()
                        {
        clear_screen();
        main_menu(); });
    tw_display.print();
}

void start()
{
    std::string line, vari;
    std::ifstream username_file, Twitt_like;
    username_file.open("../data/usernames.txt");
    if (username_file.is_open())
    {
        while (std::getline(username_file, line))
        {
            if (line != "")
            {
                User *user = new User;
                std::ifstream user_file;
                user_file.open("../data/" + line + "/user_info.txt");
                std::getline(user_file, vari);
                user->set_name(vari);
                std::getline(user_file, vari);
                user->set_family(vari);
                std::getline(user_file, vari);
                user->set_email(vari);
                std::getline(user_file, vari);
                user->set_password(vari);
                user->set_username(line);

                std::ifstream twitt_file;
                twitt_file.open("../data/" + line + "/twitts.txt");
                while (std::getline(twitt_file, vari))
                {
                    user->add_my_twitt(vari);
                }
                twitt_file.close();

                std::ifstream follower_file;
                follower_file.open("../data/" + line + "/followers.txt");
                while (std::getline(follower_file, vari))
                {
                    user->add_follower(vari);
                }
                follower_file.close();

                UserList.push_back(user);

                int user_twitt_count{0};
                for (auto twt : user->get_twitts())
                {
                    Twitt *twitt = new Twitt;
                    user_twitt_count++;
                    twitt->set_value(twt);
                    twitt->set_id(user->get_username() + "-" + std::to_string(user_twitt_count));
                    Twitts.push_back(twitt);
                }
            }
        }
        username_file.close();
    }
    Twitt_like.open("../data/Twitt_like.txt");
    if (Twitt_like.is_open())
    {
        while (std::getline(Twitt_like, line))
        {
            if (line != " ")
                for (int i = 0; i < Twitts.size(); i++)
                {
                    if (Twitts[i]->get_id() == line.substr(0, Twitts[i]->get_id().size()))
                    {
                        Twitts[i]->set_like_count(std::stoi(line.substr(Twitts[i]->get_id().size() + 1)));
                        break;
                    }
                }
        }
        Twitt_like.close();
    }
}

void close()
{
    memory_save();
    exit(0);
}
