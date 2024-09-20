# oneginSorter


This program sort text of Pushkin's poem Eugene Onegin
from ./texts/onegin.txt in alphabet and rhyme orders and
print sorted texts to ./texts/sortedOnegin in the folowwing order:

1. Alphabet sorted text.

2. Rhyme sorted text.

3. Original text.

You can sort your own texts by changing paths in ./sources/main.cpp


# Downloading (Linux)


1. Clone repository
```git clone https://github.com/Afobaboa/oneginSorter```

2. Go to root directory og
this project
```cd oneginSorter```

3. Run squareSolver
```make run```


# Make


You can expand this project, adding new .cpp
and .h files to /sources and /headers directories.
You can use ready Makefile.

1. To compile project write
```make```

2. To compile project with debugging flags and sanitizers
```make debug```

3. To run program 
```make run```

4. To clean object files for recompilation
```make clean```

5. To clean log files
```make logs_clean```