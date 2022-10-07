#include <iostream>
#include <limits>
#include <stack>
#include <vector>
#include <algorithm>
#include <string>



using namespace std;

// Struct to hold the expression
struct Expression {
  // input stack to store the expression
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

// Function to check the syntax of the expression
// Te accepted syntax includes: A-Z, + - / * ^ () with no spaces in between
// The function prints out the syntax errors with their position
// Returns true for a valid syntax
bool check_syntax(string expression ){
   // check if the string is not empty.
   if( expression.length() == 0){
      cout<<"Please provide a valid expression."<<endl;
      return false;
   }

   // number of errors
   int errors = 0;
   // Keep count of the number of opening and closing parenthesis
   int opening_parenthesis=0, closing_parenthesis=0;
  
   // check if the expression matches the required syntax
   for(int i =0; i< expression.length();i++){

      char c = expression.at(i);

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
         wcout<<"Syntax error at character ("<<c<<") position "<<(i+1)<<endl;
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
      cout<<endl<<"Compiled expression with "<<errors<<" errors"<<endl;
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

// Computes the prefix of the provided syntatically valid expression
void prefix(){

   // Prompt the user for input
   cout<<endl<<"Prefix "<<endl<<"_________________"<<endl;
   cout<<"Enter expression: "<<endl;
   
   string expression;
   cin>>expression;


   // trim expression whitespace characters
   string WHITESPACE = " \r\t\n\f\v";
   size_t trim_start = expression.find_first_not_of(WHITESPACE);
   size_t trim_end = expression.find_last_not_of(WHITESPACE);

   expression = expression.substr(trim_start, trim_end+1);

   // struct to hold the parsed expression
   struct Expression eq;

   if(check_syntax(expression)){
      //push the reversed array into the input stack
      //Since we are pushing to a stack, it is automatically reversed = LIFO
      for (int i = 0; i < expression.length(); i++)
      {
         eq.input.push(expression[i]);
      }
   }else{
      // Return if the expression has syntax errors
      return;
   }

   // show table header
   // input |  stack |  output
   // =========================
   //       |        |
   cout<<endl<<"  input\t| stack\t| output"<<endl<<"========================="<<endl;

   // Loop through the input expression until it is empty
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

// Computes the postfix of the provided syntatically valid expression
void postfix (){

   // Prompt the user for input
   cout<<endl<<"Postfix "<<endl<<"_________________"<<endl;
   cout<<"Enter expression: "<<endl;
   
   string expression;
   cin>>expression;


   // trim expression whitespace characters
   string WHITESPACE = " \r\t\n\f\v";
   size_t trim_start = expression.find_first_not_of(WHITESPACE);
   size_t trim_end = expression.find_last_not_of(WHITESPACE);

   expression = expression.substr(trim_start, trim_end+1);

   // struct to hold the parsed expression
   struct Expression eq;

   if(check_syntax(expression)){
      //push the expression on the input stack in reverse to ensure that 
      // it ends up in the same order as the input expression due to LIFO
      for (int i = expression.length()-1; i >= 0; i--)
      {
         eq.input.push(expression[i]);
      }
   }else{
      // Return if the expression has syntax errors
      return;
   }

   // show table header
   // input |  stack |  output
   // =========================
   //       |        |
   cout<<endl<<"  input\t| stack\t| output"<<endl<<"========================="<<endl;

   while(!eq.input.empty()){
      char input = eq.input.top();
      cout<<"  "<<input;

      if(input >= 'A' && input <= 'Z'){
         //goes to output
         eq.output.push_back(input);
         
      }else{
         //try to pop all operators in the operator stack until none remains higher precedence or equal precedence
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

   // pop the remaining operators and add them to the output vectors
   while (!eq.operator_stack.empty())
   {
      eq.output.push_back(eq.operator_stack.top());
      eq.operator_stack.pop();
   }

   //table footer
   cout<<"\t|\t| "<<get_vector_string(eq.output);
   cout<<endl<<"========================="<<endl;

   // print the postfix result
   string output_str = get_vector_string(eq.output);

   cout<<"Result:\t  "<<output_str<<endl;
  
}

/* ================
   Main function
 ================ */

 // main function
int main(){
  
  // int to store menu choice
  int menu_choice;

  // Infinately loop through the menu until the user exits the program
  while(true){
    // Show the menu and prompt for an option
    show_menu();
    
    // read menu choice allowing only integer values
    cin>>menu_choice;

    if(!cin){
      // Non integer value was inerted
      cout<<"Enter a valid choice or (3) to exit "<<endl;
      cin.clear();      //clear the input buffer
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      continue;
    }
    
    // check the provided menu options
    switch (menu_choice){
      case menu_prefix:
          //Prefix prompt
         prefix();
        break;
      case menu_postfix:
          //Postfix prompt
          postfix();
        break;
      case menu_exit:
          //Exit
        return 0;
      default:
         // Invalid choice
        cout<<"Invalid choice "<<endl;
        continue;
    }
  }
   
}
// We are done here 😅🚀