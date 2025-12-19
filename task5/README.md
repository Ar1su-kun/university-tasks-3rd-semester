# Task 5


## Grammar
```EBNF
<command_shell>::=<conveyor>{[one from &, &&, ||, ;] <conveyor>} [&, ;]
<conveyor>::=<command>{ | <command>}
<command>::=<name>{<arg>} [< <name>] [[one from >, >>] <name>]
```
