build_output_path = Build/
obj_path = $(build_output_path)Obj/
bin_path = $(build_output_path)Bin/
src_path = Source/
src_math_path = $(src_path)Math/
src_test_path = $(src_path)Test/
src_util_path = $(src_path)Util/
src_util_system_path = $(src_util_path)System/
cl_compiler = "cl.exe"
compiler = $(cl_compiler)
linker = "link.exe"
compiler_options = /MT /w /c
compiler_options_out = /Fo:$(obj_path)
linker_options = -defaultlib:libcmt -subsystem:console
linker_options_out = -out:$(bin_path)
obj_files = $(obj_path)*.o
lib_files = Gdi32.lib User32.lib
all : Vector.o Matrix.o Window.o Test.o Test.exe
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
Window.o : $(src_util_system_path)Window.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Window.o \
	$(src_util_system_path)Window.cpp"	
Test.o : $(src_test_path)Test.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Test.o \
	$(src_test_path)Test.cpp"	
Test.exe : $(obj_files)
	"$(linker) \
	$(linker_options) \
	$(linker_options_out)Test.exe \
	$(obj_files) $(lib_files)"
