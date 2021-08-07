bison -d -t python-s.y
flex python-s.l
gcc -o pys python-s.tab.c lex.yy.c 