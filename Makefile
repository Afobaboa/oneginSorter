CC=g++-11

FLAGS=-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall $\
-Wextra -Weffc++ -Waggressive-loop-optimizations $\
-Wc++14-compat -Wmissing-declarations -Wcast-align $\
-Wcast-qual -Wchar-subscripts -Wconditionally-supported $\
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal $\
-Wformat-nonliteral -Wformat-security -Wformat-signedness $\
-Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor $\
-Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith $\
-Winit-self -Wredundant-decls -Wshadow -Wsign-conversion $\
-Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 $\
-Wsuggest-attribute=noreturn -Wsuggest-final-methods $\
-Wsuggest-final-types -Wsuggest-override -Wswitch-default $\
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code $\
-Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix $\
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast $\
-Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation $\
-fstack-protector -fstrict-overflow -flto-odr-type-merging $\
-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 $\
-pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,$\
bounds,enum,float-cast-overflow,float-divide-by-zero,$\
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,$\
return,returns-nonnull-attribute,shift,signed-integer-overflow,$\
undefined,unreachable,vla-bound,vptr


# needed directories
SOURCES_DIR=sources/
HEADERS_DIR=headers/
OBJECTS_DIR=objects/


# needed files
SOURCES=$(wildcard $(SOURCES_DIR)*.cpp)
HEADERS=$(wildcard $(HEADERS_DIR)*.h)
OBJECTS=$(patsubst $(SOURCES_DIR)%.cpp,$(OBJECTS_DIR)%.o,$(SOURCES))
EXECUTABLE=oneginSorter


# making project EXECUTABLE
all: $(EXECUTABLE) $(OBJECTS_DIR)


# run EXECUTABLE
run: $(EXECUTABLE)
	./$<


# compilation EXECUTABLE
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $@


# compile object files
$(OBJECTS_DIR)%.o: $(SOURCES_DIR)%.cpp $(HEADERS)
	$(CC) -c $(FLAGS) $< -o $@


# make object directory
dir:
	mkdir $(OBJECTS_DIR)


# clean object files and EXECUTABLE for recompilation
clean: 
	rm -rf $(OBJECTS_DIR) $(EXECUTABLE)