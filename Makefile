CCFLAGS = -lncurses -lpthread -std=gnu++0x

all: clinicSymulator

clinicSymulator: clinicSymulator.cpp
	g++ clinicSymulator.cpp Doctor.cpp Doctor.h Patient.cpp Patient.h Reception.cpp Reception.h patientThreadFunction.h patientThreadFunction.cpp receptionThreadFunction.h receptionThreadFunction.cpp -o clinicSymulator $(CCFLAGS)
	
test: all
	./clinicSymulator 4
	
clean:
	rm -rf *.o clinicSymulator