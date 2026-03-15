CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2

TARGET = biojoin++

SRCS = biojoin.cpp create_dictionary.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

PREFIX ?= /usr/local
BIN_DIR := $(PREFIX)/bin
MAN_DIR := $(PREFIX)/share/man/man1
MANPAGE := biojoin.1

install: $(OUT) $(MANPAGE)
	install -d $(DESTDIR)$(BIN_DIR)
	install -m 0755 $(OUT) $(DESTDIR)$(BIN_DIR)
	install -d $(DESTDIR)$(MAN_DIR)
	install -m 0644 $(MANPAGE) $(DESTDIR)$(MAN_DIR)

OUT := biojoin++
MANPAGE := biojoin.1

uninstall:
	rm -f $(BIN_DIR)/$(OUT)
	rm -f $(MAN_DIR)/$(MANPAGE)


clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
