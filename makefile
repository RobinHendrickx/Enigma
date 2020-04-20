enigma: rotor.o enigma.o reflector.o plugboard.o
	g++ -Wall -g rotor.o enigma.o reflector.o plugboard.o -o enigma

rotor.o: rotor.cpp rotor.h errors.h
	g++ -Wall -g -c rotor.cpp

enigma.o: enigma.cpp rotor.h reflector.h errors.h
	g++ -Wall -g -c enigma.cpp

reflector.o: reflector.cpp reflector.h errors.h
	g++ -Wall -g -c reflector.cpp

plugboard.o: plugboard.cpp plugboard.h errors.h
	g++ -Wall -g -c plugboard.cpp

clean:
	rm -rf *.o enigma output.txt

