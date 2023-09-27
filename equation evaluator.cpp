#include <iostream>

#include <stack>

#include <string>

#include <iomanip>

#include <fstream>

#include <sstream>

using namespace std;

//checking the precedence to perform bodmass
int precedenceChecker(char operatorSymbol)
{
    if (operatorSymbol == '-' || operatorSymbol == '+')
    {
        return 1;
    }
    else if (operatorSymbol == '*' || operatorSymbol == '/')
    {
        return 2;
    }
    return 0;
}

//function to perform arithmetic operations
double doOperation(double value1, double value2, char operatorSymbol)
{
    switch (operatorSymbol)
    {
    case '+':
    {
        return value1 + value2;
    }
    case '-':
    {

        return value1 - value2;
    }
    case '/':
    {
        return value1 / value2;
    }
    case '*':
    {
        return value1 * value2;
    }
    }
}

//function to perform infix evaluation and finding the answer
string putIntoStackAndFindAnswer(string mathsEquation)
{

    //to verify whether the number of opening brackets and closing brackets are same.
    int noOfClosing = 0;
    int noOfOpening = 0;
    int noOfOpeningCurly = 0;
    int noOfClosingCurly = 0;
    int noOfOpeningSquare = 0;
    int noOfClosingSquare = 0;

    //converting various types of brackets to one type of bracket.so that we can design code for only one bracket
    for (int j = 0; j < mathsEquation.length() - 1; j++)
    {

        if (mathsEquation[j] == '{')
        {
            noOfOpeningCurly++;
            mathsEquation[j] = '(';
        }
        else if (mathsEquation[j] == '}')
        {
            noOfClosingCurly++;
            mathsEquation[j] = ')';
        }
        else if (mathsEquation[j] == '[')
        {
            noOfOpeningSquare++;
            mathsEquation[j] = '(';
        }
        else if (mathsEquation[j] == ']')
        {
            noOfClosingSquare++;
            mathsEquation[j] = ')';
        }
        else if (mathsEquation[j] == '(')
        {
            noOfOpening++;
        }
        else if (mathsEquation[j] == ')')
        {
            noOfClosing++;
        }

    }

    //returning E when the opening and closing number of brackets vary
    if (noOfClosing != noOfOpening)
    {
        return "E";
    }
    if (noOfClosingCurly != noOfOpeningCurly)
    {
        return "E";
    }
    if (noOfClosingSquare != noOfOpeningSquare)
    {
        return "E";
    }

    //creating 2 types brackets to store operators and values
    stack < double > numberStack;
    stack < char > operatorStack;

    //we do not need to consider =, that why mathsEquation.length() - 1
    for (int i = 0; i < mathsEquation.length() - 1; i++)
    {
        //to neglect unnecessary spaces if available
        if (mathsEquation[i] == ' ')
        {
            continue;
        //for opening brackets
        }
        else if (mathsEquation[i] == '(')
        {
            operatorStack.push(mathsEquation[i]);
        }
        //for numbers
        else if (isdigit(mathsEquation[i]))
        {
            int number = 0;
            while (i < mathsEquation.length() && isdigit(mathsEquation[i]))
            {
                //sometimes numbers will not have 1 digit only. so to convert that below expression is used
                number = (number * 10) + (mathsEquation[i] - '0');
                i++;
            }
            numberStack.push(number);

            //In the above while loop the string index will be offset.so to remove that offset i-- is used.
            i--;
        }
        //for closing brackets
        else if (mathsEquation[i] == ')')
        {

            while (operatorStack.top() != '(' && !operatorStack.empty())
            {
                //popping two top values and top operator form stack and doing operation
                char operatorNow = operatorStack.top();
                operatorStack.pop();
                double value2 = numberStack.top();
                numberStack.pop();
                double value1 = numberStack.top();
                numberStack.pop();
                numberStack.push(doOperation(value1, value2, operatorNow));
            }
            if (!operatorStack.empty())
            {
                operatorStack.pop();
            }
        }
        else
        {

            while (!operatorStack.empty() && precedenceChecker(operatorStack.top()) >= precedenceChecker(mathsEquation[i]))
            {
                //popping two top values and top operator form stack and doing operation
                char operatorNow = operatorStack.top();
                operatorStack.pop();
                double value2 = numberStack.top();
                numberStack.pop();
                double value1 = numberStack.top();
                numberStack.pop();
                numberStack.push(doOperation(value1, value2, operatorNow));
            }

            operatorStack.push(mathsEquation[i]);

        }

    }

    while (!operatorStack.empty())
    {
        //popping two top values and top operator form stack and doing operation
        //now lastly 2 values and a operator will only be remaining
        char operatorNow = operatorStack.top();
        operatorStack.pop();
        double value2 = numberStack.top();
        numberStack.pop();
        double value1 = numberStack.top();
        numberStack.pop();
        numberStack.push(doOperation(value1, value2, operatorNow));

    }
    //converting the double value to 2 decimal place and returning it as a string
    stringstream stream;
    stream << fixed << setprecision(2) << numberStack.top();
    string finalAnswer = stream.str();
    //checking whether the answer is infinity or not
    if (finalAnswer == "inf")
    {
        return "E";
    }

    return finalAnswer;

}

int main()
{
    //opening the formula file and reading it
    ifstream inputFile("formulas.txt");

    if (!inputFile)
    {
        cout << "Failed to open input file!" << endl;
        return 1;
    }

    string line;
    //for writing file
    fstream myFile_Handler;
    string myLine;
    //creating a new file to input answers
    ofstream MyFile("result.txt");

    myFile_Handler.open("result.txt", ios::in | ios::out);

    if (!myFile_Handler)
    {
        cout << "File did not open!";
        exit(1);
    }
    cout<<endl;
    while (getline(inputFile, line))
    {
        string result = putIntoStackAndFindAnswer(line);
        //displaying the output
        cout<<"\t"<<line<<" "<<result<<endl;
        //inputting the answer in the new file
        myFile_Handler << line << result << endl;
    }
    //closing the opened files
    cout<<endl<<"Answers are stored successfully"<<endl;
    MyFile.close();
    inputFile.close();

    return 0;
}
