# tritone

matlab-style vector expression evaluator. Uses a formal context-free grammar to build an abstract syntax tree, and uses recursive-descent parsing to evaluate the tree. 

## usage
- scalar operations: 
    - addition: `1+2`
    - subtraction: `15-3`
    - multiplication: `8*8`
    - division: `9/3`
    - pemdas: `(1+3)/(15*15)`
- vector operations: 
    - addition: `(1, 2, 3) + (4, 5, 6)`
    - subtraction: `(1, 2, 3) - (4, 5, 6)`
    - piecewise multiplication: `(1, 2, 3) * (4, 5, 6)`
    - piecewise division: `(1, 2, 3) / (4, 5, 6)`
    - dot product: `(1, 2, 3) . (4, 5, 6)`
    - cross product: `(1, 2, 3) X (4, 5, 6)`
    - assignments: 
        - `var1 = 1, 2, 3` 
        - `var1 = 1,2,3` 
        - `var1 = (1, 2, 3)X(4, 5, 6)`
- commands: 
    - `clear`: clear the screen
    - `quit`: "exits gracefully"
    - `free`: clears out the vector table
    - `help`: prints the help text
    - `list`: lists all the currently stored variables in mystery order
    - `write "path"`: writes the currently stored variables to `path`. Must be in quotes or will most definitely break.
    - `read "path"`: attempts to read `path` as a csv. `path` must be in quotes or will most definitely break. 
    - `fill <num>`: Fills the vectable with `num` vectors. May break if `num` is greater than 4.  

## implementation details

### parsing
I wrote out essentially this grammar on a piece of printer paper at 4am on a Saturday. This is pretty much what was implemented:
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
For the week 7 lab, I added a String type as a terminal symbol, but I don't necessarily know how to properly denote that in the grammar. 

### storage and IO
Variable storage is implemented as a linear-probing hash table. I tested it on my desktop and I was able to store around 1mil vectors without it breaking, but on the school laptops, it segfaults around ~2000 vectors. 


## things that were stolen from elsewhere
- [this](https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm) article by Theo Novell, which explained the construction of a CFG well enough for me to understand.
- [this](https://stackoverflow.com/questions/898489/what-programming-languages-are-context-free
) Stackoverflow post helped differentiate between different parts of the parser and what each component is responsible for
- [this](http://www.cse.yorku.ca/~oz/hash.html) is where I got the `djb2` hash implementation
 
- [this](https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c) is where I got `rand_string()` from


