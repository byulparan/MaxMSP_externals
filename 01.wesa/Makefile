MAX_SDK= /Users/byul/Code/max-sdk-7.3.3/source/c74support/max-includes
TARGET=wesa

all :
	clang -c $(TARGET).c -o $(TARGET).o -include $(MAX_SDK)/macho-prefix.pch -I $(MAX_SDK)
	clang -bundle -o $(TARGET) $(TARGET).o @$(MAX_SDK)/c74_linker_flags.txt
	mv $(TARGET) $(TARGET).mxo/Contents/MacOS/
	cp -R $(TARGET).mxo ~/Documents/Max\ 7/Library
