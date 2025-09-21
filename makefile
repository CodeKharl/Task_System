CPPC = g++
CPPFLAGS = -std=c++20 -Wall -Wextra -g -I include -MMD -MP

SRCDIR = src
BUILDIR = build
BINDIR = bin

TARGET = bin/TaskS

SOURCES = $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDIR)/%.o, $(SOURCES))

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CPPC) $(CPPFLAGS) $^ -o $@	
	@echo "Build Successfully"

$(BUILDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDIR)
	mkdir -p $(dir $@)
	$(CPPC) $(CPPFLAGS) -c $< -o $@

$(BUILDIR) $(BINDIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(BUILDIR) $(BINDIR)
	@echo "Clean Successfully"
