#include <iostream>
#include <limits>
#include <stack>
#include <vector>
#include <algorithm>
#include <string>

#define MAX 100

using namespace std;

// Struct to hold the equation
struct Equation {
  // input stack to store the equation
  stack<char> input;
  // vector to hold the output
  vector<char> output;
  // stack to hold the operators
  stack<char> operator_stack;
};


// Enum for menu choices
enum menu{
   // Prefix
   menu_prefix = 1,
   // Postfix
   menu_postfix = 2,
   // Exit
   menu_exit = 3
};

// function to print menu
void show_menu(){
  cout<<endl<<"Menu "<<endl<<"_________________"<<endl;
  cout<<"1. Prefix"<<endl<<"2. Postfix"<<endl<<"3. Exit"<<endl;
  cout<<"> ";
  
}

// Function to check the syntax of the equation
// Te accepted syntax includes: A-Z, + - / * ^ () with no spaces in between
// The function prints out the syntax errors with their position
// Returns true for a valid syntax
bool check_syntax(string equation ){
   // check if the string is not empty.
   if( equation.length() == 0){
      cout<<"Please provide a valid equation."<<endl;
      return false;
   }

   // number of errors
   int errors = 0;
   // Keep count of the number of opening and closing parenthesis
   int opening_parenthesis=0, closing_parenthesis=0;
  
   // check if the equation matches the required syntax
   for(int i =0; i< equation.length();i++){

      char c = equation[i];

      if(c>='A' && c<='Z'/* Between A-Z */){
         continue;
      }else if(c=='*' || c == '+' || c == '/' || c == '-' || c == '^'){
         // contains + - / * ^
         continue;
      }else if(c == '('){
         // opening parenthesis
         opening_parenthesis++;
         continue;
      }else if( c == ')' ){
         // closing parenthesis
         closing_parenthesis++;
         continue;
      }else{
         // Print the unrecognized error
         cout<<"Syntax error at character ("<<c<<") position "<<(i+1)<<endl;
         errors++;
      }
      
   }

   // If the count of the opening and closing parenthesis are not equal it indicates syntax error
   if(opening_parenthesis != closing_parenthesis){
      cout<<"Syntax error: Non matching parenthesis"<<endl;
      errors++;
   }

   // print and return false if any errors
   if(errors>0){
      cout<<endl<<"Compiled equation with "<<errors<<" errors"<<endl;
      cout<<"Accepted characters are A-Z, + - / * ^ ()"<<endl;
      return false;
   }

   return true;
}


// Converts vector to a printable string
string  get_vector_string(vector<char> output){
   string out;
   
   //loop through the characters appending them to the end of the output string
   for (int i = 0; i < output.size(); i++)
   {
      out.push_back(output.at(i));
   }
   return out;
}

// Converts a stack to a printable string
string get_stack_string(stack<char> output){
   string out;

   //loop through the stack poping the characters and appending them to the end of the output string
   while(!output.empty()){
      out.push_back(output.top());
      output.pop();
   }

   //Reverse the string so as to obtain the proper order of the output string due to LIFO
   reverse(out.begin(), out.end());
   return out;
}


// Calculates the precedence of the provided character
// Returns the precedence for a valid character else returns -1
int precedence(char c){
   const char opening_parenthesis = '(';     // 4
   const char exponent = '^';                // 3 
   const char division = '/';                // 2
   const char multiplication = '*';          // 2
   const char addition = '+';                // 1
   const char subtraction = '-';             // 1
   const char closing_parenthesis = ')';     // 0

   switch (c)
   {
      case opening_parenthesis:  // (
         return  4;
      case exponent:             // ^
         return 3;
      case division:             // /
         return 2;
      case multiplication:       // *
         return 2;
      case addition:             // +
         return 1;
      case subtraction:          // -
         return 1;
      case closing_parenthesis:  // )
         return 0;
      default:
         return -1;
   }
}

// compares precedence,
//  returns 0 for equal precedence
//         1 if a is greater precedence
//         -1 if a is of lower precedence 
int compare_precedence(char a, char b){
   
   if( a == b)
      return 0;   // Equal precedence
   if(precedence(a) > precedence(b))
      return 1;   // Higher precedence
   if(precedence(a) == precedence(b))
      return 0;   // Equal pecedence
   else
      return -1;  // Lower precedence
}

// Computes the prefix of the provided syntatically valid equation
void prefix(){

   // Prompt the user for input
   cout<<endl<<"Prefix "<<endl<<"_________________"<<endl;
   cout<<"Enter prefix equation: "<<endl;
   
   string equation;
   cin>>equation;


   // trim equation whitespace characters
   string WHITESPACE = " \r\t\n\f\v";
   size_t trim_start = equation.find_first_not_of(WHITESPACE);
   size_t trim_end = equation.find_last_not_of(WHITESPACE);

   equation = equation.substr(trim_start, trim_end+1);

   // struct to hold the parsed equation
   struct Equation eq;

   if(check_syntax(equation)){
      //push the reversed array into the input stack
      //Since we are pushing to a stack, it is automatically reversed = LIFO
      for (int i = 0; i < equation.length(); i++)
      {
         eq.input.push(equation[i]);
      }
   }else{
      // Return if the equation has syntax errors
      return;
   }

   // show table header
   // input |  stack |  output
   // =========================
   //       |        |
   cout<<endl<<"  input\t| stack\t| output"<<endl<<"========================="<<endl;

   // Loop through the input equation until it is empty
   while(!eq.input.empty()){
      char input = eq.input.top();
      cout<<input;

      if(input >= 'A' && input <= 'Z'){
         //goes to output
         eq.output.push_back(input);
         
      }else{
         //try to pop all operators in the operator stack until none remains higher precedence
         while (!eq.operator_stack.empty() && compare_precedence(input, eq.operator_stack.top()) == -1)
         {
            // Get the top operator and pop it
            char top = eq.operator_stack.top();
            eq.operator_stack.pop();

            //push the poped value to the output
            eq.output.push_back(top);
         }

         // Its safe to push the current operator to the output stack
         eq.operator_stack.push(input);

      }
      //print the current table state
      stack<char> op = eq.operator_stack;
      cout<<"\t| "<<get_stack_string(op)<<"\t| "<<get_vector_string(eq.output);
      cout<<endl;
      eq.input.pop();
   }

   // pop the remaining operators and add them to the output vector
   while (!eq.operator_stack.empty())
   {
      eq.output.push_back(eq.operator_stack.top());
      eq.operator_stack.pop();
   }

   cout<<"\t|\t| "<<get_vector_string(eq.output);
   cout<<endl<<"========================="<<endl;

   //Reverse the output vector to obtain the prefix stack
   string output_str = get_vector_string(eq.output);
   reverse(output_str.begin(), output_str.end());

   cout<<"Result:\t  "<<output_str<<endl;
  
}

void postfix (){
  cout<<endl<<"Postfix "<<endl<<"_________________"<<endl;
  cout<<"Enter postfix equation: "<<endl;
  
  string equation;
  cin>>equation;
   // trim equation whitespace characters
   string WHITESPACE = " \r\t\n\f\v";
   size_t trim_start = equation.find_first_not_of(WHITESPACE);
   size_t trim_end = equation.find_last_not_of(WHITESPACE);

   equation = equation.substr(trim_start, trim_end+1);

   struct Equation eq;

   if(check_syntax(equation)){
      //push the equation on the input stack in reverse to ensure that it ends up in the same order as the input equation due to LIFO
      for (int i = equation.length()-1; i >= 0; i--)
      {
         eq.input.push(equation[i]);
      }
   }else{
      return;
   }

   //show table
   cout<<endl<<"  input\t| stack\t| output"<<endl<<"========================="<<endl;

   while(!eq.input.empty()){
      char input = eq.input.top();
      cout<<"  "<<input;

      if(input >= 'A' && input <= 'Z'){
         //goes to output
         eq.output.push_back(input);
         
      }else{
         //try to pop all operators in the operator stack until none remains higher precedence
         while (!eq.operator_stack.empty() && (compare_precedence(input, eq.operator_stack.top()) == -1 || compare_precedence(input, eq.operator_stack.top()) == 0))
         {
            // Get the top operator and pop it
            char top = eq.operator_stack.top();
            eq.operator_stack.pop();

            //push the poped value to the output
            eq.output.push_back(top);
         }

         // Its safe to push the current operator to the output stack
         eq.operator_stack.push(input);

      }
      //print the current table state
      stack<char> op = eq.operator_stack;
      cout<<"\t| "<<get_stack_string(op)<<"\t| "<<get_vector_string(eq.output);
      cout<<endl;
      eq.input.pop();
   }

   while (!eq.operator_stack.empty())
   {
      eq.output.push_back(eq.operator_stack.top());
      eq.operator_stack.pop();
   }

   cout<<"\t|\t| "<<get_vector_string(eq.output);
   cout<<endl<<"========================="<<endl;

   string output_str = get_vector_string(eq.output);

   cout<<"Result:\t  "<<output_str<<endl;
  
}
int main(){
  
  //show menu;
  int menu_choice;
  while(true){
    show_menu();
    
    //read menu choice allowing only integer values
    cin>>menu_choice;
    if(!cin){
      cout<<"Enter a valid choice or (3) to exit "<<endl;
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      continue;
    }
    
    switch (menu_choice){
      case menu_prefix:
          //Prefix stuff
         prefix();
        break;
      case menu_postfix:
          //Postfix stuff
          postfix();
        break;
      case menu_exit:
          //Exit
        return 0;
      default:
        cout<<"Invalid choice "<<endl;
        continue;
    }
  }
  string equation;
  
   cin>>equation;
   
   cout<<equation.length()<<endl;
   
   for(int i = 0; i < equation.length(); i++){
     
   }
   
}