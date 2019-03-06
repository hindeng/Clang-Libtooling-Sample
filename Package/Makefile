# This works for both binary installed and source installed
# It seems that clang++ has some problem, so g++ is used.
# Note that I add -std=c++11 option here to support smart pointer
CXX := g++
CXXFLAGS := -fno-rtti -O0 -g -w -std=c++11 \
-I/root/llvm-project/build/include \
-I/root/llvm-project/llvm/include \
-I/root/llvm-project/build/tools/clang/include \
-I/root/llvm-project/clang/include \
-I/root/xuan-project/Package

# Your own LLVM build path
LLVM_DIR := /root/llvm-project/build

# LLVM C++ options and LLVM link options
LLVM_CXXFLAGS := `${LLVM_DIR}/bin/llvm-config --cxxflags`
LLVMLDFLAGS := `${LLVM_DIR}/bin/llvm-config --ldflags --libs --system-libs`


SOURCES = Example.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXES = $(OBJECTS:.o=)

CLANGLIBS = \
				-lclangTooling\
				-lclangFrontendTool\
				-lclangFrontend\
				-lclangDriver\
				-lclangSerialization\
				-lclangCodeGen\
				-lclangParse\
				-lclangSema\
				-lclangStaticAnalyzerFrontend\
				-lclangStaticAnalyzerCheckers\
				-lclangStaticAnalyzerCore\
				-lclangAnalysis\
				-lclangARCMigrate\
				-lclangRewriteFrontend\
				-lclangRewrite\
				-lclangEdit\
				-lclangAST\
				-lclangLex\
				-lclangBasic\
				$(shell $(LLVMCONFIG) --libs)\
				$(shell $(LLVMCONFIG) --system-libs)\
                

all:	$(OBJECTS) $(EXES)

#Not obvious, see my blog
%: %.o
	$(CXX) -w -o $@ $< $(CLANGLIBS) $(LLVMLDFLAGS)

.PHONY: all clean

#make clean manually
clean:
	rm -f $(OBJECTS) $(EXES) *.o
