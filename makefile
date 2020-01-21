#
# makefile
#
# Copyright 2019 Rickie S. Kerndt <kerndtr@kerndt.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#
#
#

CPP := g++ -std=c++11
CC  := gcc -std=c99

CFLAGS := -Os -finline-functions -W -Wall
#CFLAGS := -g -W -Wall

TARGETS := md5 bsd-md5 mddriver MD5Hash-test

all: $(TARGETS)

MD5.o: MD5.cpp MD5.h
	$(CPP) $(CFLAGS) -c MD5.cpp

MD5.s: MD5.cpp MD5.h
	$(CPP) $(CFLAGS) -S MD5.cpp

main.o: main.cxx MD5.h
	$(CPP) $(CFLAGS) -c main.cxx

md5: main.o MD5.o
	$(CPP) $(CFLAGS) -o md5 main.o MD5.o

bsd-md5: bsd-md5.c
	$(CC) $(CFLAGS) -o bsd-md5 bsd-md5.c -L/usr/lib/libbsd.so -lbsd

openwell-md5.o: openwell-md5.c openwell-md5.h
	$(CC) $(CFLAGS) -c openwell-md5.c

mddriver.o: mddriver.c openwell-md5.h
	$(CC) $(CFLAGS) -c mddriver.c

mddriver: mddriver.o openwell-md5.o
	$(CC) $(CFLAGS) -o mddriver mddriver.o openwell-md5.o

MD5Hash.o: MD5Hash.cpp MD5Hash.h
	$(CPP) $(CFLAGS) -c MD5Hash.cpp

MD5Hash-test.o: MD5Hash-test.cxx MD5Hash.h
	$(CPP) $(CFLAGS) -c MD5Hash-test.cxx

MD5Hash-test: MD5Hash-test.o MD5Hash.o MD5.o MD5Hash.h MD5.h
	$(CPP) $(CFLAGS) -o MD5Hash-test MD5Hash-test.o MD5Hash.o MD5.o

clean:
	@rm -f *.o *.s

realclean:
	@rm -f *.o *.s $(TARGETS)

.PHONY: clean realclean

