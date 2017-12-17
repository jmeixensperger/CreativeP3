P3.out : Project2.o CodeGenerator.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P3.out Project2.o CodeGenerator.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h CodeGenerator.h
	g++ -g -c SyntacticalAnalyzer.cpp

CodeGenerator.o : CodeGenerator.cpp CodeGenerator.h LexicalAnalyzer.h
	g++ -g -c CodeGenerator.cpp

clean : 
	rm *.o P3.out *.lst *.dbg *.p1 *.gch


submit : Project2.cpp CodeGenerator.h CodeGenerator.cpp Object.h Object.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf CreativeP3
	mkdir CreativeP3
	cp Project2.cpp CreativeP3
	cp CodeGenerator.h CreativeP3
	cp CodeGenerator.cpp CreativeP3
	cp Object.h CreativeP3
	cp Object.cpp CreativeP3
	cp LexicalAnalyzer.h CreativeP3
	cp LexicalAnalyzer.cpp CreativeP3
	cp SyntacticalAnalyzer.h CreativeP3
	cp SyntacticalAnalyzer.cpp CreativeP3
	cp makefile CreativeP3
	cp README.txt CreativeP3
	tar cfvz CreativeP3.tgz CreativeP3
	cp CreativeP3.tgz ~tiawatts/cs460drop
