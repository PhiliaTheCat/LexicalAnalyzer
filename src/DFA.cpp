#include <iostream>

#include "../include/DFA"
#include "../include/Macros"

namespace ptc
{
    bool num(const char &start, std::ifstream &fin, char *const buffer)
    {
        int i = 0;
        buffer[i] = start;
        i += 1;
        char p;
        bool flag = true;
        while (1)
        {
            fin.get(p);
            if (fin.eof())
            {
                buffer[i] = 0;
                return true;
            }
            if (p < 33) // white space and control symbol
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
            else if (p >= '0' && p <= '9' || p == '.') // digits and dot
            {
                if (flag && p == '.')
                    flag = false;
                buffer[i] = p;
                i += 1;
            }
            else if (p == ';') // endline
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
            else if
            (
                p == '+' || p == '-' || p == '*' || p == '/'
                || p == '=' || p == '<' || p == '>'
            ) // arithmetic operators, comparison operators, assignment operator
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
            else if (p == '&' || p == '|') // logic operators expect '!'
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
            else if (p == ')') // right round bracket
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
            else if
            (
                p == '(' ||
                p >= 'A' && p <= 'Z'||
                p >= 'a' && p <= 'z' ||
                p == '!' ||
                p == '{' || p == '}'
            ) // left round bracket, letters and logic not
            {
                buffer[i] = p;
                i += 1;
                buffer[i] = 0;
                return false;
            }
            else // undefined
            {
                buffer[i] = p;
                i += 1;
                buffer[i] = 0;
                return false;
            }
        }
    }

    bool comp_and_assign(const char &start, std::ifstream &fin, Token &src)
    {
        int status = 0;
        char p = start;
        do 
        {
            if (status == 0)
            {
                if (p == '<')
                    status = 1;
                else if (p == '=')
                    status = 3;
                else // if (p == '>')
                    status = 2;
            }
            else if (status == 1)
            {
                if (p == '=')
                    status = 5;
                else if (p < 33)
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = LT;
                    fin.unget();
                    return true;
                }
                else if
                (
                    p >= '0' && p <= '9' ||
                    p >= 'A' && p <= 'Z' || 
                    p >= 'a' && p <= 'z'
                )
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = LT;
                    fin.unget();
                    return true;
                }
                else if (p == ' ')
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = LT;
                    fin.unget();
                    return true;
                }
                else
                    return false;
            }
            else if (status == 2)
            {
                if (p == '=')
                    status = 4;
                else if (p < 33)
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = GT;
                    fin.unget();
                    return true;
                }
                else if
                (
                    p >= '0' && p <= '9' ||
                    p >= 'A' && p <= 'Z' || 
                    p >= 'a' && p <= 'z'
                )
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = GT;
                    fin.unget();
                    return true;
                }
                else if (p == ' ')
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = GT;
                    fin.unget();
                    return true;
                }
                else
                    return false;
            }
            else if (status == 3)
            {
                if (p == '=')
                    status = 6;
                else if (p < 33)
                {
                    src.attr = ASSIGN_OPERATOR;
                    fin.unget();
                    return true;
                }
                else if
                (
                    p >= '0' && p <= '9' ||
                    p >= 'A' && p <= 'Z' || 
                    p >= 'a' && p <= 'z'
                )
                {
                    src.attr = ASSIGN_OPERATOR;
                    fin.unget();
                    return true;
                }
                else if (p == ' ')
                {
                    src.attr = ASSIGN_OPERATOR;
                    fin.unget();
                    return true;
                }
                else if (p == '(' || p == '+' || p == '-' || p == '!')
                {
                    src.attr = ASSIGN_OPERATOR;
                    fin.unget();
                    return true;
                }
                else
                    return false;
            }
            else if (status == 4)
            {
                if (p < 33)
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = GTE;
                    fin.unget();
                    return true;
                }
                else if
                (
                    p >= '0' && p <= '9' ||
                    p >= 'A' && p <= 'Z' || 
                    p >= 'a' && p <= 'z'
                )
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = GTE;
                    fin.unget();
                    return true;
                }
                else if (p == ' ')
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = GTE;
                    fin.unget();
                    return true;
                }
                else if (p == '(' || p == '+' || p == '-' || p == '!')
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = GTE;
                    fin.unget();
                    return true;
                }
                else
                    return false;
            }
            else if (status == 5)
            {
                if (p < 33)
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = LTE;
                    fin.unget();
                    return true;
                }
                else if
                (
                    p >= '0' && p <= '9' ||
                    p >= 'A' && p <= 'Z' || 
                    p >= 'a' && p <= 'z'
                )
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = LTE;
                    fin.unget();
                    return true;
                }
                else if (p == ' ')
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = LTE;
                    fin.unget();
                    return true;
                }
                else if (p == '(' || p == '+' || p == '-' || p == '!')
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = LTE;
                    fin.unget();
                    return true;
                }
                else
                    return false;
            }
            else if (status == 6)
            {
                if (p < 33)
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = EQ;
                    fin.unget();
                    return true;
                }
                else if
                (
                    p >= '0' && p <= '9' ||
                    p >= 'A' && p <= 'Z' || 
                    p >= 'a' && p <= 'z'
                )
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = EQ;
                    fin.unget();
                    return true;
                }
                else if (p == ' ')
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = EQ;
                    fin.unget();
                    return true;
                }
                else if (p == '(' || p == '+' || p == '-' || p == '!')
                {
                    src.attr = COMPARISON_OPERATOR;
                    src.val = EQ;
                    fin.unget();
                    return true;
                }
                else
                    return false;
            }
            else 
                return false;
            fin.get(p);
            if (fin.eof())
                break;
        }
        while (1);
        return false;
    }

    bool id_and_key(const char &start, std::ifstream &fin, char *const buffer)
    {
        int i = 0;
        buffer[i] = start;
        i += 1;
        char p = 0;
        int status = 0;
        while (1)
        {
            fin.get(p);
            if (fin.eof())
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
            if
            (
                p >= 'A' && p <= 'Z' ||
                p >= 'a' && p <= 'z' ||
                p == '_' ||
                p >= '0' && p <= '9'
            )
            {
                buffer[i] = p;
                i += 1;
            }
            else if (p < 33)
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
            else if (p == '+' || p == '-' || p == '*' || p == '/')
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
            else if (p >= 34 && p <= 37 || p == 39 || p == 46 || p == 58 || p == 63)
            {
                buffer[i] = p;
                i += 1;
                buffer[i] = 0;
                return false;
            }
            else
            {
                buffer[i] = 0;
                fin.unget();
                return true;
            }
        }
    }

} // namespace ptc
