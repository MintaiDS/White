obj_path = Obj/
bin_path = Bin/
src_path = Source/
src_math_path = $(src_path)Math/
cl_compiler = "cl.exe"
compiler = $(cl_compiler)
linker = "link.exe"
compiler_options = /MT /w /c
compiler_options_out = /Fo:$(obj_path)
linker_options = -defaultlib:libcmt -subsystem:console
linker_options_out = -out:$(bin_path)
obj = $(obj_path)*.obj
all : Vector.o Matrix.o
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
