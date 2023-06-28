#pragma once

#include <iostream>
#include <exception>
#include <string>
#include <stack>

bool isoperator(char c){
  return c == '+' || c == '-' || c == '*' || c == '/';
}

bool isoperator(std::string c){
  return c == "+" || c == "-" || c == "*" || c == "/";
}


int GetOperatorPrecedence(std::string c){
  if (c == "+" || c == "-") return 1;
  else if (c == "*" || c == "/") return 2;
  else return 0;
}

float ToFloat(std::string Num) {
  return std::stof(Num);
}

void ParseOperator(std::string Op, std::stack<std::string>& MainStack, std::stack<std::string>& OpStack){
  if (OpStack.empty() || GetOperatorPrecedence(OpStack.top()) < GetOperatorPrecedence(Op)) OpStack.push(Op);
  else if (!OpStack.empty() && GetOperatorPrecedence(OpStack.top()) >= GetOperatorPrecedence(Op)){
    while (!OpStack.empty() && GetOperatorPrecedence(OpStack.top()) >= GetOperatorPrecedence(Op)) {
      MainStack.push(OpStack.top());
      OpStack.pop();
    }
    OpStack.push(Op);
  }
  else std::cout << "Error\n";
}

double applyOperator(const std::string& op, double operand1, double operand2) {
  if (op == "+")
    return operand1 + operand2;
  else if (op == "-")
    return operand1 - operand2;
  else if (op == "*")
    return operand1 * operand2;
  else if (op == "/")
    return operand1 / operand2;
  else
    return 0.0;  // Invalid operator
}

double EvaluatePostFix(std::stack<std::string>& postfixExpression) {
  std::stack<double> OperandStack;
  std::stack<std::string> InvertedPostFix;

  double operand1, operand2;
  
  while(!postfixExpression.empty()){
    InvertedPostFix.push(postfixExpression.top());
    postfixExpression.pop();
  }

  
  while(!InvertedPostFix.empty()){
    if(!isoperator(InvertedPostFix.top())) {
      OperandStack.push(std::stod(InvertedPostFix.top()));
      InvertedPostFix.pop();
    }else {
      if (OperandStack.size() < 2) throw std::out_of_range("Number of operands too low.");
      operand2 = OperandStack.top();
      OperandStack.pop();
      operand1 = OperandStack.top();
      OperandStack.pop();
      OperandStack.push(applyOperator(InvertedPostFix.top(), operand1, operand2));
      InvertedPostFix.pop();
    }  
  }

  if (OperandStack.size() == 1) return OperandStack.top();
  else throw std::out_of_range("Out of operands.");

}

double EvaluateExpression(const std::string& Exp){
  std::stack<std::string> RStack;
  std::stack<std::string> OpStack;

  std::string Num = "";
  for (char c : Exp) {
    if (isdigit(c)){
      Num += c;
    }else if (isoperator(c)){
      if (Num != ""){
        RStack.push(Num);
        Num = "";
      }
      Num += c;
      ParseOperator(Num, RStack, OpStack);
      Num = "";
    }else{
      if (Num == "") continue;
      RStack.push(Num);
      Num = "";
    }
  }
  if (Num != "") RStack.push(Num);
  Num = "";
  while (!OpStack.empty()){
    RStack.push(OpStack.top());
    OpStack.pop();
  }

  return EvaluatePostFix(RStack);
}
