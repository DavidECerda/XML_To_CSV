CXX = g++

INCLUDEDIR = ./include
CJNITTADIR = /home/cjnitta/ecs34
SRCDIR = ./src
OBJDIR = ./obj
CURDIR = $(shell pwd)
LIBDIR = ./lib
LIBCSVDIR = libcsv-3.0.3
LIBCSV_NAME = libcsv.a

INCLUDE = -I $(INCLUDEDIR)
CXXFLAGS = -std=c++14 -Wall $(INCLUDE)

CSV_READER_OBJ = $(OBJDIR)/CSVReader.o
CSV_WRITER_OBJ = $(OBJDIR)/CSVWriter.o
CSV_TEST_OBJ = $(OBJDIR)/testcsv.o
XML_READER_OBJ = $(OBJDIR)/XMLReader.o
XML_WRITER_OBJ = $(OBJDIR)/XMLWriter.o
XML_TEST_OBJ = $(OBJDIR)/testxml.o

OBJS	= $(CSV_READER_OBJ)	\
		  $(CSV_WRITER_OBJ) \
		  $(XML_READER_OBJ) \
		  $(XML_WRITER_OBJ) \
		  $(OBJDIR)/StringUtils.o \
		  $(OBJDIR)/Path.o \
		  $(LIBDIR)/$(LIBCSV_NAME) 

all: directories $(LIBDIR)/$(LIBCSV_NAME) bin/studconv tests 

#From previous projects' Makefiles, by Nitta	
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/studconv: obj/main.o $(OBJS)
	$(CXX) $(CXXFLAGS) obj/main.o $(OBJS) -lexpat -o bin/studconv

directories:
	mkdir -p ./obj
	mkdir -p ./bin
	mkdir -p ./testbin
	# mkdir -p ./exxml
	# mkdir -p ./excsv
	
tests: testbin/testcsv testbin/testxml
	testbin/testcsv
	testbin/testxml

testbin/testcsv: $(CSV_READER_OBJ) $(CSV_WRITER_OBJ) $(CSV_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(CSV_READER_OBJ) $(CSV_WRITER_OBJ) $(CSV_TEST_OBJ) $(LIBDIR)/$(LIBCSV_NAME) -o testbin/testcsv -lgtest -lgtest_main -lpthread

testbin/testxml: $(XML_READER_OBJ) $(XML_WRITER_OBJ) $(XML_TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(XML_READER_OBJ) $(XML_WRITER_OBJ) $(XML_TEST_OBJ) -lexpat -o testbin/testxml -lgtest -lgtest_main -lpthread


$(LIBCSVDIR)/Makefile:
	cd $(LIBCSVDIR); chmod 755 configure; cd ..
	cd $(LIBCSVDIR); ./configure --prefix=$(CURDIR); cd ..
	
$(LIBCSVDIR)/libcsv.la: $(LIBCSVDIR)/Makefile
	cd $(LIBCSVDIR); make; cd ..
	
$(LIBDIR)/$(LIBCSV_NAME): $(LIBCSVDIR)/libcsv.la
	cd $(LIBCSVDIR); make install; cd ..

# testfiles: excsv exxml

# excsv: directories
# 	cp -r $(CJNITTADIR)/proj4samples/*.csv $(CURDIR)/excsv/ ;

# exxml: directories
# 	cp -r $(CJNITTADIR)/proj4samples/*.xml $(CURDIR)/exxml/ ;

clean:
	rm -rf lib/
	rm -rf share/
	rm -f include/csv.h
	cd $(LIBCSVDIR); make clean; cd ..
	rm -rf $(LIBCSVDIR)/Makefile
	rm -rf obj/* testbin/* bin/*
	rm -rf exxml/ excsv/
