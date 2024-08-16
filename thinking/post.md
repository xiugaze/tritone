# Technical Explanation

The stated problem was to build a command-line interface for evaluating simple mathematical expressions, i.e. 1+2, 2+4, 9*9, etc. I soon realized that while this is a fairly trivial ask on the surface (i.e. all that building something that got the grade would require would be a little `strtok`-ing and `atoi`s), building something *correct* was a different challenge altogether.

For the CLI calculator to be useful, we need to be able to anticipate a lot of different user input. We want to be able to accept `1+1`, but we want to extend that functionality more generally, to things like `1+ 1` or `1 + 1` or `1     + 1`. Actually, we'd really want to be able to extend it to text like `1 + 1 - 1`, or `1*2 - 2*(3 + 4)`. 

I realized that what I really wanted to make here was an expression evaluator. I wanted to generalize the interface to where expressions that adhere to a syntax will be evaluated, regardless of length or complexity. But what is a syntax anyway?

## Formal Languages
Computers are machines that like to adhere to a strict set of rules, and a certain subset of the people that like to make computers also like to make other things adhere to strict sets of rules. Some people who are much smarter than me came up with some ideas about sets of strings and tokens, and on how to write rules on how they can be arranged.

A formal language is a (possibly infinite) set of strings that follow some set of rules. Particularly, a formal language has two things
- an alphabet, which is a set of indivisible symbols or characters. An alphabet, like any set, can be finite, countably infinite, or uncountably infinite. The symbols in the alphabet can really be anything (we could have an alphabet where the set of symbols that it consists of is the set of all real numbers, or of all atoms in the universe, or the set containing all other sets), but for most computer-related purposes, this alphabet is just the set of printable characters (e.g. ASCII). 
- syntax, which is a set of rules that define the ways in which the symbols in the alphabet can be arranged to be a part of the language. If a string of symbols from the alphabet follows the syntax, it is in the language. 

Strings of symbols from the alphabet are called words, and if a word follows the syntax of the language, we call it a *well-formed word* or a *well-formed formula (wff)*. 

For instance, we could define an alphabet $\Sigma$ as:
$$
\Sigma = \{a, b\}
$$

Using this as our alphabet, we could define a language $L$ as:
$$
L = \{aa, bb\}
$$
For this made-up language $L$, the string $aa$ is in the language, so it's a well-formed formula. However, $ab$ is *not* in the language, even though it's made up of symbols from our alphabet $\Sigma$. If a language $L$ is a set of strings made up of symbols from $\Sigma$, we'd say that we have a language $L$ *over* $\Sigma$. 

Just for completion, linguists will define $\Sigma^*$ as the infinite set of all possible combinations of the symbols in the alphabet $\Sigma$. So, for our friend $\Sigma = \{a, b\}$, $\Sigma^*$ would look something like

$$
\Sigma^* = \{a, b, aa, bb, ab, ba, aaa, ... \}
$$

It follows that all languages $L$ over $\Sigma$ are subsets of $\Sigma^*$. 
The size of the set of all possible languages, or the number of possible languages using a given alphabet $\Sigma$ is $|\mathcal{P}(\Sigma^*)| = 2^{|\Sigma^*|}$. 

Cool, okay. That was a whole lot of a set theory with a whole little of utility. We know what an alphabet is and what a language is. yadda yadda transition...

## Grammars

A formal grammar is a list of rules that describe what strings from an alphabet get to belong to a language. From before, we defined a language $L$ as 
$$
L = \{a, b\}
$$
We could notate the grammar for $L$ like:

1. $S -> a$
2. $S -> b$

Here, we call $S$ the *start* or *sentence* symbol. These rules are called *production rules*, because can produce any and all well-formed words in $L$ by starting with $S$ and applying the rules in $G$. If we follow the first rule, we end up with $a$, and if we follow the second rule, we end up with $b$. 
A grammar is kind of like coming up with a set-builder for the language. For $L$, it's easy to describe the language as a finite set, and it feels kind of unnecessary to describe a grammar for it. 

Grammars become useful when we're trying to come up with ways to describe *infinite* languages. For instance, we could describe 
$\Sigma^{*}$ using the following rules: 

1. $S->aS$
2. $S->bS$
3. $S->\epsilon$


### Quick aside
A grammar $G = (N, \Sigma, P, S)$ is
- a finite set $N$ of nonterminal symbols (symbols than can be replaced through a production rule, syntactic variables ), that is disjoint with the strings formed from $G$. 
- a finite set $\Sigma$ of terminal symbols (symbols that can't be replaced by a production rule) that is disjoint from N
- A finite set $P$ of production rules, each of the form $(\Sigma \cup N)^{*}N(\Sigma \cup N)^{*} -> (\Sigma \cup N)^{*}$ where $*$ is the Kleene star operator and $\cup$ is union. 
- A denoted start symbol $S \in N$. 
