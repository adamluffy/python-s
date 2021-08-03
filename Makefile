bison -d python-s.y
flex python-s.l
gcc -o pys python-s.tab.c lex.yy.c 
rm python-s.tab.* lex.yy.c 