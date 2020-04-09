TARGET = cable-cal-factor-rw
HEADER = cable-cal-factor-rw.h util.h
SOURCE = test.cpp cable-cal-factor-rw.cpp util.cpp
LINKFLAG = -l gmock_main -l gmock -l gtest

all : ${TARGET} run

${TARGET}: ${HEADER} ${SOURCE}
	g++ -std=c++0x -o ${TARGET} ${SOURCE} ${LINKFLAG}

run: ${TARGET}
	./${TARGET}

clean:
	rm -f ./${TARGET}
