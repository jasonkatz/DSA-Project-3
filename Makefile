spell.exe: main.o Hash.o
	g++ -o spell.exe main.o Hash.o

spellcheck.o: main.cpp Hash.h
	g++ -c main.cpp

Hash.o: Hash.cpp Hash.h
	g++ -c Hash.cpp

debug:
	g++ -g -o spellDebug.exe main.cpp Hash.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
