OS =		$(shell uname -s)
TARGET =	measure_elapsed
CPPFLAGS =	-std=c++11
CPPFLAGS += 	-O2 -Wall -Weffc++ -pedantic -pedantic-errors -Wextra			\
		-Waggregate-return -Wcast-align -Wcast-qual -Wchar-subscripts		\
		-Wcomment -Wconversion -Wdisabled-optimization -Werror -Wfloat-equal	\
		-Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security		\
		-Wformat-y2k -Wimplicit -Wimport -Winit-self -Winline -Winvalid-pch	\
		-Wlong-long -Wmissing-braces -Wmissing-field-initializers		\
		-Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn	\
		-Wpacked -Wpadded -Wparentheses -Wpointer-arith -Wredundant-decls	\
		-Wreturn-type -Wsequence-point -Wshadow -Wsign-compare			\
		-Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch	\
		-Wswitch-default -Wswitch-enum -Wtrigraphs -Wuninitialized		\
		-Wunknown-pragmas -Wunreachable-code -Wunused -Wunused-function		\
		-Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable	\
		-Wvariadic-macros -Wvolatile-register-var -Wwrite-strings
CPP =		clang++
OBJS =		$(patsubst %.cc, %.o, $(wildcard *.cc))

# OS-specific configuration
ifeq ($(OS), Darwin)
CPPFLAGS +=	-stdlib=libc++
LDFLAGS =	-lc++
else
CPPFLAGS +=	-stdlib=libstdc++
LDFLAGS +=	-lstdc++ -static
endif


.PHONY: all
all: $(TARGET)

%.o: %.cc
	$(CPP) $(CPPFLAGS) -c $<

$(TARGET): $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $(OBJS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

