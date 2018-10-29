# ExpressionTree
This is simply implementaion for ExpressionTree, which can be used for calculate value of an expression that only contains basic oprerators( +,-,*,/) and trigonometric functions(sin,tan,cos,cot,arcSin,arcTan,arcCos,arcCot) and does not contains digits. This jerkwater library just suit for using in school task, small project because it is not fully function. 

Example:

#include "ExpressionTree.h"

using namespace std;

int main()

{

   ExpressionTree expression;
   
   expression.setExpression("(y+(sin(a)+t)/d)");
   
   //then set parameter values
   
   map<string,double> values;
   
   values["y"]=76.222;
   
   values["a"]=45.3;
   
   values["t"]=7.33;
   
   values["d"]=8;
   
   cout<<expression.getValues(values);
   
   return 0;
   
}
