
parser: lex.yy.c lolfx.tab.c
	$(CXX) $^ -o $@ -lfl

lolfx.tab.c: lolfx.y
	bison -d $^

lex.yy.c: lolfx.l
	flex -d $^

