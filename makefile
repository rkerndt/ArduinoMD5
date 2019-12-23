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

CC := g++ -std=c++11
#CFLAGS := -O3 -finline-functions -W -Wall
CFLAGS :=  -W -Wall

TARGETS := md5-test

all: $(TARGETS)

MD5.o: MD5.cpp MD5.h
	$(CC) $(CFLAGS) -c MD5.cpp

MD5.s: MD5.cpp MD5.h
	$(CC) $(CFLAGS) -S MD5.cpp

main.o: main.cxx MD5.h
	$(CC) $(CFLAGS) -c main.cxx

md5-test: main.o MD5.o
	$(CC) $(CFLAGS) -o md5-test main.o MD5.o

clean:
	@rm -f *.o *.s

realclean:
	@rm -f *.o *.s $(TARGETS)

.PHONY: clean realclean
