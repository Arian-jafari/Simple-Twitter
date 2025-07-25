#include "../includes/Twitter.h"
#include <iostream>

void print_tips() {
    std::cout << "Please notice to following notes about MyTwitter:\n";
    std::cout << "1- If program asks for 'yes', enter 'yes' to agree or anything else to skip.\n";
    std::cout << "2- Use the 'Close' option to exit and save changes.\n";
    std::cout << "3- Press any key to continue when paused.\n";
}

int main() {
    start();
    CubbyMenu::Menu menu;
    menu.add_header("MyTwitter");
    menu.add_item("Sign up", &sign_up);
    menu.add_item("Sign in", &sign_in);
    menu.add_item("Some tips about MyTwitter", [](){
        clear_screen();
        print_tips();
        CubbyMenu::Menu Back;
        Back.add_item("Back", []() {
            clear_screen();
            CubbyMenu::Menu menu;
            menu.add_header("MyTwitter");
            menu.add_item("Sign up", &sign_up);
            menu.add_item("Sign in", &sign_in);
            menu.add_item("Some tips about MyTwitter", [](){
                print_tips();
            });
            menu.add_item("Close", &close);
            menu.print();
        });
        Back.print();
    });

    menu.add_item("Close", &close);
    menu.print();
}
