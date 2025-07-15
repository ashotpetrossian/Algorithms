#include <vector>
#include <stack>
#include <string>
#include <iostream>

/*
    Shunting Yard Algorithm - Infix to Postfix Converter

    Motivation: LeetCode 1597. Build Binary Expression Tree From Infix Expression

    This implementation uses Dijkstra's Shunting Yard algorithm to convert an infix mathematical expression
    (e.g., "3 + 4 * 2 / (1 - 5)") into a postfix (Reverse Polish Notation) expression (e.g., "3 4 2 * 1 5 - / +").

    Supported features:
    - Basic binary operators: +, -, *, /
    - Operator precedence and left-to-right associativity
    - Parentheses for grouping
    - Multi-digit integer support
    - Whitespace is ignored

    Note:
    - This implementation assumes valid input expressions.
    - Mismatched parentheses are not explicitly handled (but can be added).
    - Unary operators (e.g., unary minus) are not supported in this version.

     ---
    About the name:
    The "Shunting Yard" algorithm was invented by Edsger W. Dijkstra — the same computer scientist
    who created the famous Dijkstra shortest path algorithm. The name refers to a railway shunting yard,
    where train cars (representing tokens in an expression) are rearranged from an incoming track (infix)
    to a new ordering on an outgoing track (postfix), using temporary side tracks (the operator stack).

    How it works:
    - Operands (numbers) are sent directly to the output.
    - Operators are temporarily held in a stack to respect operator precedence and associativity.
    - Higher-precedence operators must appear earlier in postfix, so lower-precedence operators must wait.
    - When a closing parenthesis is found, operators are popped until the matching opening parenthesis.
    - At the end, any remaining operators are flushed into the output.

    The operator stack is essential for delaying the output of operators until it is guaranteed that
    no higher-precedence operators remain pending, ensuring correct postfix structure.
*/

// infix to postfix converter
struct ShuntingYardSolver
{
public:
    ShuntingYardSolver(const std::string& s) : expr{ s }
    {
    }

private:
    int precedence(char c)
    {
        if (c == '*' || c == '/') return 2;
        if (c == '+' || c == '-') return 1;
        return 0; // for '('
    }

public:
    std::vector<std::string> solve()
    {
        std::vector<std::string> output;
        std::stack<char> operators;
        int n = expr.size();

        for (int i{}; i < n; ++i) {
            char c{ expr[i] };
            if (c == ' ') continue;

            if (std::isdigit(c)) {
                std::string num(1, c);
                ++i;
                while (i < n && std::isdigit(expr[i])) {
                    num.push_back(expr[i++]);
                }
                --i;

                //std::cout << "Number: " << num << std::endl;
                output.push_back(num);
            }
            else if (c == '(') {
                //std::cout << "Pushing open bracket into operators stack\n";
                operators.push(c);
            }
            else if (c == ')') {
                //std::cout << "Found closing bracket, pushing operators into output until an open bracket\n";
                while (!operators.empty() && operators.top() != '(') {
                    output.push_back(std::string(1, operators.top()));
                    operators.pop();
                }
                if (!operators.empty()) operators.pop(); // popping '('
            }
            else { // found an operator
                //std::cout << "Found operator: " << c << std::endl;
                while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                    //std::cout << "top = " << operators.top() << " so pushing top to the output\n";
                    output.push_back(std::string(1, operators.top()));
                    operators.pop();
                }

                operators.push(c);
            }

            //std::cout << "After current operation the output: ";
            //for (const std::string& s : output) std::cout << s << " ";
            //std::cout << std::endl;
        }

        // pushing all the remaining operators into the output
        while (!operators.empty()) {
            output.push_back(std::string(1, operators.top()));
            operators.pop();
        }

        return output;
    }

private:
    std::string expr;
};

void test1()
{
    std::string s{ "1 + 3 * 4 - 2" };
    ShuntingYardSolver solver{ s };

    auto res = solver.solve();
    std::cout << "Infix: " << s << std::endl;
    for (const std::string& s : res) std::cout << s << " ";
    std::cout << "\n" << std::endl;
}

void test2()
{
    std::string s{ "3+(4*2)-1" };
    ShuntingYardSolver solver{ s };

    auto res = solver.solve();
    std::cout << "Infix: " << s << std::endl;
    for (const std::string& s : res) std::cout << s << " ";
    std::cout << "\n" << std::endl;
}

void test3()
{
    std::string s{ "2-3/(5*2)+1" };
    ShuntingYardSolver solver{ s };

    auto res = solver.solve();
    std::cout << "Infix: " << s << std::endl;
    for (const std::string& s : res) std::cout << s << " ";
    std::cout << "\n" << std::endl;
}

void test4()
{
    std::string s{ "3*4-2*5" };
    ShuntingYardSolver solver{ s };

    auto res = solver.solve();
    std::cout << "Infix: " << s << std::endl;
    for (const std::string& s : res) std::cout << s << " ";
    std::cout << "\n" << std::endl;
}

void test5()
{
    std::string s{ "1+2+3+4+5" };
    ShuntingYardSolver solver{ s };

    auto res = solver.solve();
    std::cout << "Infix: " << s << std::endl;
    for (const std::string& s : res) std::cout << s << " ";
    std::cout << "\n" << std::endl;
}

void test6()
{
    std::string s{ "3 + 4 * 2 / (1 - 5)" };
    ShuntingYardSolver solver{ s };

    auto res = solver.solve();
    std::cout << "Infix: " << s << std::endl;
    for (const std::string& s : res) std::cout << s << " ";
    std::cout << "\n" << std::endl;
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
}
