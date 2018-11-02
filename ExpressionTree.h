#ifndef IOSTREAM
#include<iostream>
#define IOSTREAM
#endif
#ifndef MAP
#include<map>
#define MAP
#endif
#ifndef MATH
#include<math.h>
#define MATH
#endif
#ifndef STACK
#include<stack>
#define STACK
#endif
#ifndef VECTOR
#include<vector>
#define VECTOR
#endif
#ifndef STRING
#include<string>
#define STRING
#endif
using namespace std;

class Node
{
public:
	string data;
	Node*Left;
	Node*Right;
	Node()
	{
		this->Left = this->Right = NULL;
	}
};
class ExpressionTree
{
private:
	double const PI = 3.14159265359;
	Node* Root;          //cây nhị phân này chỉ cần nắm bắt nút gốc, các nút còn lại thì dựa vào nút gốc
	void _addToExpression(Node*&R, vector<string> str)
	{
		str = toPostfix(str);
		stack<Node*> stackNode;
		for (int i = 0; i < str.size(); i++)
		{
			if (isOperand(str[i]))
			{
				Node*P = new Node;
				P->data = str[i];
				stackNode.push(P);
			}
			else
			{
				if (isOperator(str[i]))
				{
					Node*P = new Node;
					P->data = str[i];
					P->Right = stackNode.top();
					stackNode.pop();
					P->Left = stackNode.top();
					stackNode.pop();
					stackNode.push(P);
				}
			}
		}
		R = stackNode.top();
	}
	double _getInsideTerm(string trigonometricFuntion, map<string, double>& values)
	{
		string insideTerm ;
		for (int i = 0; i < trigonometricFuntion.size(); i++)
		{
			if (trigonometricFuntion[i] == '(')
			{
				for (int j = i + 1; j < trigonometricFuntion.size(); j++)
				{
					if (trigonometricFuntion[j] == ')') break;
					insideTerm += trigonometricFuntion[j];
				}
				break;
			}
		}
		//TODO: get value of the expression which has been taken out.
		ExpressionTree temp;
		temp.setExpression(insideTerm);
		double test = temp.getValue(values);
		values[trigonometricFuntion] = test;
		return test;
		//return values[insideTerm];
	}
	double _getTrigonomometricValue(string term, map<string, double>& values)
	{
		string specialWord[9] = { "sin", "cos", "tan", "cot", "arcSin", "arcCos", "arcTan", "arcCot","sqrt"};
		for (int i = 0; i < 9; i++)
		{
			int index = indexOfWord(specialWord[i], term, 0);
			if (index != -1)
			{
				if (i == 0) values[term] = sin(_getInsideTerm(term,values));
				if (i == 1) values[term] = cos(_getInsideTerm(term, values));
				if (i == 2) values[term] = tan(_getInsideTerm(term, values));
				if (i == 3) values[term] = 1/tan(_getInsideTerm(term, values));
				if (i == 4) values[term] = asin(_getInsideTerm(term, values));
				if (i == 5) values[term] = acos(_getInsideTerm(term, values));
				if (i == 6) values[term] = atan(_getInsideTerm(term, values));
				if (i == 7) values[term] = atan(1/_getInsideTerm(term, values)); 
				if (i == 8) values[term] = sqrt(_getInsideTerm(term, values));
			    return values[term];
			}
		}
		return values[term];
	}
	double _getValue(Node*&node, map<string, double>& values)  //get value of the expression
	{
		if (node == NULL) return -99999999;  //error
		string data = node->data; //data can be operands(a,b,c,...), or operator(+,-,*,/)
		if (!node->Left&&!node->Right)
		{
			//if node is sin, cos,...then calculate values[node->data] and return it
			return _getTrigonomometricValue(data, values);
		}
		if (isOperator(data))
		{
			if (data == "+")
			{
				return _getValue(node->Left, values) + _getValue(node->Right, values);
			}
			if (data == "-")
			{
				return _getValue(node->Left, values) - _getValue(node->Right, values);
			}
			if (data == "*")
			{
				return _getValue(node->Left, values) * _getValue(node->Right, values);
			}
			if (data == "/")
			{
				return _getValue(node->Left, values) / _getValue(node->Right, values);
			}
		}
	}
	bool isOperator(string x)
	{
		return (x == "+" || x == "-" || x == "*" || x == "/");
	}
	bool isOperator(char x)
	{
		return (x == '+' || x == '-' || x == '*' || x == '/');
	}
	int getPriority(string x)
	{
		if (x == "+" || x == "-") return 1;
		if (x == "*" || x == "/") return 2;
		return 0;
	}
	bool isOperand(string x) //TODO
	{
		/*if ((x > 64 && x<91) || (x>96 && x < 123)) return true;*/
		if (isOperator(x)) return false;
		if (x == "(" || x == ")"||x==" ") return false;
		return true;
	};
	vector<string> toPostfix(vector<string> A)
	{
		stack<string> opStack;       //The stack is used for containing operators as ( ) + - * /
		vector<string> result;//result under Postfix form 
		for (int i = 0; i < A.size(); i++)
		{
			if (A[i] == "(")  //nếu là dấu mở ngoặc thì thêm vào stack 
			{
				opStack.push(A[i]);
			}
			else
			{
				if (isOperand(A[i]))   //if meet operands a, bb,ccsd,dd,eew,f... then add to the "result"
				{
					result.push_back(A[i]);
				}
				else
				{
					if (isOperator(A[i]))  //nếu là toán tử thì đưa hết những toán tử có độ ưu tiên cao hơn A[i] vào result, rồi thêm A[i]vào stack 
					{
						while (!opStack.empty() && getPriority(opStack.top()) >= getPriority(A[i]))
						{
							result.push_back(opStack.top());
							opStack.pop();
						}
						opStack.push(A[i]);
					}
					else
					{
						if (A[i] == ")")
						{
							while (!opStack.empty() && opStack.top() != "(")
							{
								result.push_back(opStack.top());
								opStack.pop();
							}
							opStack.pop();
						}
					}
				}
			}
		}
		while (!opStack.empty())   //take out ramining characters from stack and push back result
		{
			result.push_back(opStack.top());
			opStack.pop();
		}
		return result;
	}
	void _setExpression(vector<string> v)
	{
		_addToExpression(this->Root, v);
	}
	int indexOfWord(string word, string sentence, int fromIndex)  //return -1 if word is not found in the sentence
	{
		int wordSize = word.size();
		int sentenceSize = sentence.size();
		for (int i = fromIndex; i <= sentenceSize - wordSize; i++)
		{
			bool isDuplicate = true;
			for (int j = 0; j < wordSize; j++)
			{
				if (word[j] != sentence[i + j])
				{
					isDuplicate = false;
					break;
				}
			}
			if (isDuplicate == true)
			{
				return i;
			}
		}
		return -1;
	}
public:
	ExpressionTree()
	{
		this->Root = NULL;
	}
	double getValue(map<string, double>& values)
	{
		return _getValue(this->Root,values);
	}
	void setExpression(string formula)
	{
		//first, add '_' character to preposition of "sin","cos",...
		string specialWord[9] = { "sin", "cos", "tan", "cot", "arcSin", "arcCos", "arcTan", "arcCot","sqrt" };

		for (int j = 0; j < 9; j++)
		{
			string word = specialWord[j];
			for (int i = 0; i < formula.size(); i++)
			{
               int startAt = indexOfWord(word, formula,i);
		       if (startAt != -1)
			   {
				 formula.insert(startAt, "_");
				 i = startAt + 2;
			   }
			}
		}
		cout << formula << endl; 
		//split the formula into small pieces by origin oder
		vector<string> pieces;
		for (int i = 0; i < formula.size();)
		{
			if (formula[i] == ' ') 
			{
				i++;
				continue;
			}
			if (formula[i] == '_') //signal of trigonometric functions
			{
				string temp;
				int j = i + 1;
				for (; formula[j] != ')';j++)
				{
					temp += formula[j];
				}
				temp += ')';
				i = j+1;
				pieces.push_back(temp);
				continue;
			}
			if (formula[i] == '(' || formula[i] == ')')
			{
				string temp;
				temp += formula[i];
				pieces.push_back(temp);
				i++;
				continue;
			}
			if (isOperator(formula[i]))
			{
				string temp;
				temp+=(formula[i]);
				pieces.push_back(temp);
				i++;
				continue;
			}
			//else, next term will be operand
			string operand;
			int j = i;
			for (; j < formula.size(); j++)
			{
				if (isOperator(formula[j]) == true) break;
				if (formula[j] == ')' || formula[j] == ' ') break;
				operand += formula[j];
			}
			i = j;
			pieces.push_back(operand);
		}
		_setExpression(pieces);
	}
	
};


