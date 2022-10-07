# Infix to Postfix or Prefix
The following program converts expressions to prefix or post fix. It provides a simple menu to select the rquired operation and then proceeds to prompt the user to provide an expressions. It checks if the syntax is correct. If the syntax is incorrect it proceeds to compute the postfix or prefix of the expression.

## Menu
The menu provides three options:
1. **Prefix** - prefix operations
2. **Postfix** - postfix operations
3. **Exit** - exit the program

![Menu Screenshot](./assets/Screenshot%20from%202022-10-07%2022-45-06.png)

## Expressions
An expression is a mathematical statement without equal sign. Expressions have a strict syntax to be followed.
The allowed characters include `() / * - + A-Z` 
The program checks the syntax before carrying out the operations.  An example of a valid expression is `A+B/C*D-E^F` space and parenthesis are currently not handled and will be correctly parsed in the future versions.
The program tries to output problematic characters and their position.

![Syntax error screenshot](./assets/Screenshot%20from%202022-10-07%2023-35-38.png)

## Postfix
`A*B+C/D^E-F`

![Postfix screenshot](./assets/Screenshot%20from%202022-10-07%2023-42-17.png)

## Prefix
`A*B+C/D^E-F`

![Prefix screenshot](./assets/Screenshot%20from%202022-10-07%2023-44-19.png)
