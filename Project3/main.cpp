#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <set>

class invalid_password : public std::exception
{
private:
    std::vector<std::string> problems_list;
    std::string out_what;

public:
    invalid_password(
        const std::vector<std::string>& problems_list, 
        const char *base_msg = "During password parsing errors were found"
    ) : std::exception(base_msg), problems_list(problems_list) {}

    const char* what()
    {
        out_what = std::string(std::exception::what()) + ": ";

        for (auto t = this->problems_list.begin(); t != this->problems_list.end(); t++)
            out_what += "\n    * " + *t;

        return out_what.c_str();
    }
};

const static std::set<char> special_symb_set = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '-'};
#define IS_IN_RANGE(x, l, u) ((x >= l) && (x <= u))

void check_password(std::string& pswd)
{
    std::vector<std::string>  problems_list;
    bool has_char = false, has_upper = false, has_number = false, has_special = false;
    for (auto t = pswd.begin(); t != pswd.end(); t++)
    {
        has_char |= IS_IN_RANGE(*t, 'A', 'Z') | (*t, 'a', 'z');
        has_upper |= IS_IN_RANGE(*t, 'A', 'Z');
        has_number |= IS_IN_RANGE(*t, '0', '9');
        has_special |= special_symb_set.find(*t) != special_symb_set.end();
    }

    if (!has_char)
        problems_list.push_back("Password must contain at least 1 character");
    if (!has_upper)
        problems_list.push_back("Password must contain at least 1 upper case character");
    if (!has_number)
        problems_list.push_back("Password must contain at least 1 number");
    if (!has_special)
        problems_list.push_back("Password must contain at least 1 special symbol (!@#$%^&*()_+-)");
    if (!IS_IN_RANGE(pswd.size(), 8, 12))
        problems_list.push_back("Password length must be from 8 to 12 characters long");

    if (problems_list.size())
        throw invalid_password(problems_list);
}

int main()
{
    std::string inp_str;

    while (true)
    {
        std::cout << "Please enter your password: ";
        std::cin >> inp_str;

        try
        {
            check_password(inp_str);
        }
        catch (invalid_password& e)
        {
            std::cerr << "Error: " << e.what() << '\n';
            continue;
        }

        break;
    }

    std::cout << "Password: '" << inp_str << "' is satisfying all requirements" << std::endl;

    return 0;
}