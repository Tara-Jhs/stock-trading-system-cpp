CXX = g++
CXXFLAGS = -std=c++20 -g -Wall -Wextra -I./include 
SRCDIR = src
OBJDIR = objects
BINDIR = .
EXECUTABLE = $(BINDIR)/UTTrade

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SOURCES))

all: $(OBJDIR) $(EXECUTABLE)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(@) $(^) 

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(<) -o $(@)

clean:
	rm -rf $(OBJDIR)
