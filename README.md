# vector-lab-v1-xiugaze

matlab-style vector expression evaluator. Uses a formal context-free grammar to build an abstract syntax tree, and uses recursive-descent parsing to evaluate the tree. 

Grammar is defined as: 
```
 * Let G := 
 *  1. <statement> := <statement><statement> 
 *  2. <statement> := <assignment> | <expression>
 *  3. <assignment> := <identifier> = <expression> 
 *  4. <expression> := <term> | <term> { + | - } <expression> 
 *  5. <term> := <factor> | <factor> { * | / | .| X } <term>
 *  6. <factor> := <identifier> | <vector> | <constant> |(<expression>)
 *  7. <identifier> := [a-zA-Z]+
 *  8. <value> := { <constant> | <constant>, <constant>, <constant> }
```
