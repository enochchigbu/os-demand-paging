#    Single Programmer Affidavit
#    I the undersigned promise that the submitted assignment is my own work. While I was
#    free to discuss ideas with others, the work contained is my own. I recognize that
#    should this not be the case; I will be subject to penalties as outlined in the course
#    syllabus.
#    Name: Enoch Chigbu
#    Red ID: 827078077
#
#    Name: Farhan Talukder
#    Red ID: 827061320


# CXX Make variable for compiler
CC=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
CCFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = wsclock.o map.o level.o pagetable.o main.o vaddr_tracereader.o log_helpers.o

# Program name
PROGRAM = demandpaging

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CXX) -o $(PROGRAM) $^

main.o : main.cpp
	$(CC) $(CCFLAGS) main.cpp
	
wsclock.o : wsclock.cpp wsclock.h
	$(CC) $(CCFLAGS) wsclock.cpp

map.o : map.cpp map.h
	$(CC) $(CCFLAGS) map.cpp

level.o: level.cpp level.h
	$(CC) $(CCFLAGS) level.cpp
    
pagetable.o: pagetable.cpp pagetable.h
	$(CC) $(CCFLAGS) pagetable.cpp

log_helpers.o: log_helpers.c log_helpers.h
	$(CC) $(CCFLAGS) log_helpers.c

vaddr_tracereader.o: vaddr_tracereader.cpp vaddr_tracereader.h
	$(CC) $(CCFLAGS) vaddr_tracereader.cpp

# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
# As we use gnuemacs which leaves auto save files termintating
# with ~, we will delete those as well.
clean :
	rm -f *.o *~ $(PROGRAM)
