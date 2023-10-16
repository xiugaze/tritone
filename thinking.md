- right now, we have a use after free when we evalue the AST and assign
constant/identifier names to node values

```c
token* tok;
tok->name = malloc(sizeof(char) * 10);
strcpy(tok->name, "test\0");

node* n = malloc(sizeof(node));
n->name = tok->name;


free(tok->name);
// n->name is now invalid/freed



```

- change statement() -> assignment() to statement() -> identifier() ->  command() | assignment()