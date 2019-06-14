#ifndef CSV__H_
#define CSV__H_

#include <string>

using namespace std;

char CSV_COMMA = 44;
char CSV_QUOTE = 34;

string         quoteCsv   (string input);
string         unquoteCsv (string input);
vector<string> splitCsv   (string input);
string         makeCsv    (vector<string> & input);

string quoteCsv(string input)
{
    if (input.find(CSV_COMMA) == string::npos) return input;

    stringstream os;
    os << "\"";
    for (auto ch : input)
    {
        os << ch;
        if (ch == CSV_QUOTE) os << ch;
    }
    os << "\"";
    return os.str();
};

string unquoteCsv(string input)
{
    if (input.find(CSV_COMMA) == string::npos) return input;
    if (*(input.begin()) != CSV_QUOTE && *(input.rbegin()) != CSV_QUOTE) return input;

    stringstream os;
    auto ch  = input.begin(); ch++;
    auto end = input.end();   end--;
    for (; ch != end ; ch++)
    {
        if (*ch == CSV_QUOTE) ch++;
        if (ch == input.end()) break;
        os << *ch;
    }
    return os.str();
};

vector<string> splitCsv(string input)
{
    vector<string> result;

    string::iterator front = input.begin();
    string::iterator back  = front;

    bool quote_flag  = false;
    int  quote_count = 0;

    while (front != input.end())
    {
        if (*front == CSV_COMMA) front++;
        if (*front == CSV_COMMA)
        {
            result.push_back("");
            front++;
            continue;
        }

        if (*front == CSV_QUOTE) {quote_count = 1; quote_flag = true;}

        back = front;
        while (true) 
        {
            back++;
            if (*back == CSV_QUOTE) 
                ++quote_count;

            if (*back == CSV_COMMA && quote_flag && (quote_count % 2 == 0)) 
                break;

            if (*back == CSV_COMMA && !quote_flag) 
                break;

            if(back == input.end()) 
                break;
        }

        quote_flag  = false;
        quote_count = 0;
        
        result.push_back(unquoteCsv(string(front, back)));
        front = back;
    }
    return result;
};

string makeCsv(vector<string> & input)
{
    stringstream os;
    for( auto val = input.begin() ; val != input.end() ; val++)
    {
        os << quoteCsv(*val);
        if (val + 1 != input.end())
            os << ",";
    }
    return os.str();
};

#endif
