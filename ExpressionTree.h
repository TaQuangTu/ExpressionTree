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
	double _getInsideTerm(string trigonometricFuntion, map<string, double> values)
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
		return values[insideTerm];
	}
	double _getTrigonomometricValue(string term, map<string, double> values)
	{
		string specialWord[8] = { "sin", "cos", "tan", "cot", "arcSin", "arcCos", "arcTan", "arcCot" };
		for (int i = 0; i < 8; i++)
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
				if (i == 7) values[term] = atan(1/_getInsideTerm(term, values)); //TODO: is this right?
			    return values[term];
			}
		}
		return values[term];
	}
	double _getValue(Node*&node, map<string, double> values)  //get value of the expression
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
		stack<string> opStack;       //stack dùng để chứa các toán tử ( ) + - * /
		vector<string> result;//tạo một chuỗi trống, chuỗi này sẽ là chuỗi được trả về
		for (int i = 0; i < A.size(); i++)
		{
			if (A[i] == "(")  //nếu là dấu mở ngoặc thì thêm vào stack 
			{
				opStack.push(A[i]);
			}
			else
			{
				if (isOperand(A[i]))   //nếu là toán hạng a, b,c,d,e,f... thì thêm vào chuỗi result
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
		while (!opStack.empty())   //trong stack còn bao nhiêu thì cứ thêm vào
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
	double getValue(map<string, double> values)
	{
		return _getValue(this->Root,values);
	}
	void setExpression(string formula)
	{
		//first, add '_' character to preposition of "sin","cos",...
		string specialWord[8] = { "sin", "cos", "tan", "cot", "arcSin", "arcCos", "arcTan", "arcCot" };

		for (int j = 0; j < 8; j++)
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


