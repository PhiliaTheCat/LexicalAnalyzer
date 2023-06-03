#include "../include/DFA"
#include "../include/Macros"

using namespace std;
using namespace ptc;

id_list_element id_list[1000];
const_list_element const_list[1000];

string attr_arr[15] = 
{
    "number", "identifier", "(", ")", "{",
    "}", "=", ";", "arithmetic", "comp",
    "logic", "type", "bool const", "branch", "print"
};

string key_arr[27] = 
{
    "", "+", "-", "*",
    "/", "<", "<=", ">"
    ">=", "" , "==", "",
    "", "", "", "",
    "&", "|", "!", "int", 
    "float", "bool", "true", "false",
    "if", "else", "print"
};

Token token_list[2000];

int id_list_length = 0;
int const_list_length = 0;

template<class ListType> bool find(ListType list[], const int &n, Token &src, const std::string &literal);
template<class ListType> void insert(ListType list[], int &n, Token &src, const std::string &literal);

int main()
{
    ifstream fin;
    fin.open("test/test_in.in", ios::in);
    ofstream fout;
    fout.open("test/test_out.out", ios::out);

    bool global_acc = true;

    char p;
    int i = 0;

    while (1)
    {
        fin.get(p);
        if (fin.eof())
            break;
        if (p < 33)
            continue;
        if (p >= '1' && p <= '9' || p == '.') // number
        {
            char *buffer = new char [1024];
            if (num(p, fin, buffer))
            {
                string literal(buffer);
                token_list[i].attr = NUMBER;
                if (!find(const_list, const_list_length, token_list[i], literal))
                    insert(const_list, const_list_length, token_list[i], literal);
                delete [] buffer;
                i += 1;
            }
            else
            {
                fout << "Head + " << i << " Token" << endl;
                fout << "Error: " << "\"" << buffer << " is not a number" << endl;
                delete [] buffer;
                global_acc = false;
                break;
            }
        }
        else if (p == '+' || p == '-' || p == '*' || p == '/') // arithmetic operator
        {
            token_list[i].attr = ARITHMETIC_OPERATOR;
            if (p == '+')
                token_list[i].val = -(ADD);
            else if (p == '-')
                token_list[i].val = -(MINUS);
            else if (p == '*')
                token_list[i].val = -(MULT);
            else // if (p == '/')
                token_list[i].val = -(DIV);
            i += 1;
        }
        else if (p == '<' || p == '>' || p == '=') // comparison operator, assignment operator
        {
            if (!comp_and_assign(p, fin, token_list[i]))
            {
                fout << "Head + " << i << " Token" << endl;
                fout << "Error: Not a valid operator" << endl;
                global_acc = false;
                break;
            }
            i += 1;
        }
        else if (p == '(' || p == ')' || p == '{' || p == '}')
        {
            if (p == '(')
                token_list[i].attr = L_R_BRACKET;
            else if (p == ')')
                token_list[i].attr = R_R_BRACKET;
            else if (p == '{')
                token_list[i].attr = L_C_BRACKET;
            else // if (p == '}')
                token_list[i].attr = L_C_BRACKET;
            i += 1;
        }
        else if (p == ';')
        {
            token_list[i].attr = ENDLINE;
            i += 1;
        }
        else if (p == '&' || p == '|' || p == '!')
        {
            token_list[i].attr = LOGIC_OPERATOR;
            if (p == '&')
                token_list[i].val = -(AND);
            else if (p == '|')
                token_list[i].val = -(OR);
            else if (p == '!')
                token_list[i].val = -(NOT);
        }
        else if (p >= 'a' && p <= 'z' || p >= 'a' && p <= 'z' || p == '_')
        {
            char *buffer = new char [1024];
            if (id_and_key(p, fin, buffer))
            {
                string literal(buffer);
                int j = 19;
                while (j < 27)
                {
                    if (literal == key_arr[j])
                        break;
                    j += 1;
                }
                if (j == 27)
                {
                    token_list[i].attr = IDENTIFER;
                    if (!find(id_list, id_list_length, token_list[i], literal))
                        insert(id_list, id_list_length, token_list[i], literal);
                    delete [] buffer;
                    i += 1;
                }
                else
                {
                    if (j >= -(INT) && j <= -(BOOL))
                        token_list[i].attr = TYPE;
                    else if (j >= -(TRUE) && j <= -(FALSE))
                        token_list[i].attr = BOOL_CONST;
                    else // if (j >= -(IF) && j <= -(ElSE))
                        token_list[i].attr = BOOL_CONST;
                    token_list[i].val = j;
                }
            }
            else
            {
                fout << "Head + " << i << " Token" << endl;
                fout << "Error: " << "\"" << buffer << " is not a number" << endl;
                delete [] buffer;
                global_acc = false;
                break;
            }
        }
    }

    if (global_acc)
    {
        fout << "Total Tokens: " << i << endl;
        fout << "Total Identifiers: " << id_list_length << endl;
        fout << "Total Contants: " << const_list_length << endl;
        fout << "Token List: " << endl;
        fout << "Notice: For idertifiers and numeric constants, " << endl;
        fout << "        val is a pointer to the no of related list." << endl;
        fout << "        For keywords, val is the literal string of it." << endl;
        fout << "<no, attribute, val>" << endl;
        for (int j = 0; j < i; j += 1)
        {
            fout << "<" << j << ", ";
            if (token_list[j].attr == NUMBER || token_list[j].attr == IDENTIFER)
                fout << attr_arr[token_list[j].attr] << ", " << token_list[j].val;
            else if (token_list[j].attr >= L_R_BRACKET && token_list[j].attr <= ENDLINE)
                fout << attr_arr[token_list[j].attr] << ", ";
            else 
                fout << attr_arr[token_list[j].attr] << ", " << key_arr[token_list[j].val];
            fout << ">" << endl;
        }
    }
    else
        fout << "Analysis Terminated" << endl;

}

template<class ListType> bool find(ListType list[], const int &n, Token &src, const std::string &literal)
{
    int l = 0, r = n;
    while (r - l > 1)
    {
        int m = l + (r - l) / 2;
        if (literal >= list[m].literal)
            l = m;
        else 
            r = m;
    }
    if (literal == list[l].literal)
    {
        src.val = l;
        return true;
    }
    else 
        return false;
}

template<class ListType> void insert(ListType list[], int &n, Token &src, const std::string &literal)
{
    n += 1;
    if (n == 1)
    {
        list[0].literal = literal;
        list[0].idx = n - 1;
        src.val = 0;
        return;
    }
    for (int i = n - 2; i > -1; i -= 1)
    {
        list[i + 1] = list[i];
        if (literal < list[i + 1].literal)
        {
            list[i].literal = literal;
            list[i].idx = n - 1;
            src.val = i;
            break;
        }
        else 
        {
            list[i + 1].literal = literal;
            list[i + 1].idx = n - 1;
            src.val = i + 1;
            break;
        }
    }
}
