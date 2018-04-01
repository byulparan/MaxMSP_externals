MAX_SDK= /Users/byul/Code/max-sdk-7.3.3/source/c74support/max-includes
MSP_SDK= /Users/byul/Code/max-sdk-7.3.3/source/c74support/msp-includes
STK_SDK= /Users/byul/Code/stk-4.6.0
TARGET=wesa_stk_jcrev~

all :
	clang++ -std=c++14 -c $(TARGET).cpp -o $(TARGET).o -include $(MAX_SDK)/macho-prefix.pch -I $(MAX_SDK) -I $(MSP_SDK) -I $(STK_SDK)/include
	clang++ -bundle -o $(TARGET) $(TARGET).o $(STK_SDK)/src/libstk.a @$(MAX_SDK)/c74_linker_flags.txt -F $(MSP_SDK) -framework MaxAudioAPI
	mv $(TARGET) $(TARGET).mxo/Contents/MacOS/
	cp -R $(TARGET).mxo ~/Documents/Max\ 7/Library
