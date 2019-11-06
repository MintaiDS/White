build_output_path = Build/
obj_path = $(build_output_path)Obj/
bin_path = $(build_output_path)Bin/
src_path = Source/
src_test_path = $(src_path)Test/
src_util_path = $(src_path)Util/
src_engine_path = $(src_path)Engine/
src_engine_render_path = $(src_engine_path)Render/
src_util_system_path = $(src_util_path)System/
src_util_math_path = $(src_util_path)Math/
src_engine_render_util_path = $(src_engine_render_path)Util/
src_engine_render_core_path = $(src_engine_core_path)Core/
cl_compiler = "cl.exe"
compiler = $(cl_compiler)
linker = "link.exe"
compiler_options = /MT /w /c
compiler_options_out = /Fo:$(obj_path)
compiler_options_include = /I $(src_util_math_path) \
                           /I $(src_util_system_path) \
						   /I $(src_engine_render_util_path) 
linker_options = -defaultlib:libcmt -subsystem:console
linker_options_out = -out:$(bin_path)
obj_files = $(obj_path)*.o
lib_files = Gdi32.lib User32.lib
all : Vector.o Matrix.o Window.o Canvas.o Test.o Test.exe
Matrix.o : $(src_util_math_path)Matrix.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Matrix.o \
	$(compiler_options_include) \
	$(src_util_math_path)Matrix.cpp"	
Vector.o : $(src_util_math_path)Vector.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Vector.o \
	$(compiler_options_include) \
	$(src_util_math_path)Vector.cpp"	
Window.o : $(src_util_system_path)Window.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Window.o \
	$(compiler_options_include) \
	$(src_util_system_path)Window.cpp"	
Test.o : $(src_test_path)Test.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Test.o \
	$(compiler_options_include) \
	$(src_test_path)Test.cpp"	
Canvas.o : $(src_engine_render_util_path)Canvas.cpp
	"$(compiler) \
	$(compiler_options) \
	$(compiler_options_out)Canvas.o \
	$(compiler_options_include) \
	$(src_engine_render_util_path)Canvas.cpp"	
Test.exe : $(obj_files)
	"$(linker) \
	$(linker_options) \
	$(linker_options_out)Test.exe \
	$(obj_files) $(lib_files)"
