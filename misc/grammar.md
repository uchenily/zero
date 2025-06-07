# 语法说明


```
program -> <declaration>* END
```

## 声明(Declarations)

```
declaration -> <var_declaration> | <func_declaration> | <statement>
```

```
var_declaration -> "let" <IDENTIFIER> ("=" <expression>)? ";"
```

```
func_declaration -> "fn" <IDENTIFIER> "(" <parameters>? ")" <block>
```

## 语句(Statements)

```
statement -> <if_stmt> | <for_stmt> | <while_stmt> | <block> | <return_stmt> | <expr_stmt>
```

```
if_stmt -> "if" "(" <expression> ")" <statement> ( "else" <statement> )?
```

```
for_stmt -> "for" "(" ( <var_decl> | <expr_stmt> | ";" ) <expression>? ";" <expression>? ")" <statement>
```

```
while_stmt -> "while" "(" <expression> ")" <statement>
```

```
expr_stmt -> <expression> ";"
```

```
block -> "{" <declaration>* "}"
```

```
return_stmt -> "return" <expression>? ";"
```

## 表达式(Expressions)

```
expression -> <assignment>
```

```
assignment -> <IDENTIFIER> "=" <assignment> | <logic_or>
```

```
logic_or -> <logic_and> ( "or" <logic_and> )*
```

```
logic_and -> <equality> ( "and" <equality> )*
```

```
equality -> <comparison> ( ( "!=" | "==" ) <comparison> )*
```

```
comparison -> <term> ( ( ">" | ">=" | "<" | "<=" ) <term> )*
```

```
term -> <factor> ( ( "-" | "+" ) <factor> )*
```

```
factor -> <unary> ( ( "/" | "*" ) <unary> )*
```

```
unary -> ( "!" | "-" ) <unary> | <call>
```

```
call -> <primary> ( "(" <arguments>? ")" )*
```

```
primary -> <NUMBER> | <STRING> | "false" | "true" | "nil" | <IDENTIFIER> | "(" <expression> ")"
```

## 其他

```
parameters -> <IDENTIFIER> ( "," <IDENTIFIER> )*
```

```
arguments -> <expression> ( "," <expression> )*
```
