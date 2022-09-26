CC = g++
CFLAGS = -Wall -g 

LDFLAGS = -lGL -lGLU -lglut -lGLEW

objs = main.o shader.o solid.o objParser.o
                                                                                                                                                               

main:  $(objs)
	$(CC) $(CFLAGS)  -o main $(objs) $(LDFLAGS)
 
# The main.o target can be written more simply
 
main.o:  shader.hpp solid.hpp objParser.hpp
	$(CC) $(CFLAGS) -c main.cpp 
 
shader.o:  shader.hpp
	$(CC) $(CFLAGS) -c shader.cpp 

solid.o:  solid.hpp objParser.hpp
	$(CC) $(CFLAGS) -c solid.cpp 

objParser.o:  objParser.hpp 
	$(CC) $(CFLAGS) -c objParser.cpp 


.PHONY: clean
clean: 
	rm main $(objs)