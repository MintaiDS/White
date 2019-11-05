build_output_path = Build/
obj_path = $(build_output_path)Obj/
bin_path = $(build_output_path)Bin/
src_path = Source/
src_math_path = $(src_path)Math/
src_test_path = $(src_path)Test/
cl_compiler = "cl.exe"
compiler = $(cl_compiler)
linker = "link.exe"
compiler_options = /MT /w /c
compiler_options_out = /Fo:$(obj_path)
linker_options = -defaultlib:libcmt -subsystem:console
linker_options_out = -out:$(bin_path)
obj = $(obj_path)*.o
all : Vector.o Matrix.o Test.o Test.exe
Matrix.o : $(src_math_path)Matrix.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Matrix.o \
	$(src_math_path)Matrix.cpp"	
Vector.o : $(src_math_path)Vector.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Vector.o \
	$(src_math_path)Vector.cpp"	
Test.o : $(src_test_path)Test.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Test.o \
	$(src_test_path)Test.cpp"	
Test.exe : $(obj)
	"$(linker) \
	$(linker_options) \
	$(linker_options_out)Test.exe \
	$(obj)"
