# Disassemble the shared library
objdump -d libs/$1.so

# Show all symbols
objdump -t libs/$1.so
nm -D libs/$1.so