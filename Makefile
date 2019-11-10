build_output_path = Build/
obj_path = $(build_output_path)Obj/
bin_path = $(build_output_path)Bin/
src_path = Source/
external_path = External/
external_gl_path = $(external_path)OpenGL/
src_test_path = $(src_path)Test/
src_util_path = $(src_path)Util/
src_engine_path = $(src_path)Engine/
src_engine_graphics_path = $(src_engine_path)Graphics/
src_util_system_path = $(src_util_path)System/
src_util_math_path = $(src_util_path)Math/
src_engine_graphics_util_path = $(src_engine_graphics_path)Util/
src_engine_graphics_core_path = $(src_engine_graphics_path)Core/
src_engine_graphics_gl_path = $(src_engine_graphics_path)GL/
src_engine_graphics_shader_path = $(src_engine_graphics_path)Shader/
src_engine_graphics_drawing_path = $(src_engine_graphics_path)Drawing/
cl_compiler = "cl.exe"
compiler = $(cl_compiler)
linker = "link.exe"
compiler_options = /MT /w /c
compiler_options_out = /Fo:$(obj_path)
compiler_options_include = /I $(src_util_math_path) \
                           /I $(src_util_system_path) \
                           /I $(src_engine_graphics_util_path) \
                           /I $(src_engine_graphics_core_path) \
                           /I $(src_engine_graphics_gl_path) \
                           /I $(src_engine_graphics_drawing_path) \
                           /I $(src_engine_graphics_shader_path) \
                           /I $(external_gl_path)
linker_options = -defaultlib:libcmt -subsystem:windows
linker_options_out = -out:$(bin_path)
obj_files = $(obj_path)*.o
lib_files = Gdi32.lib User32.lib Opengl32.lib
all : engine
engine: Vector.o Common.o Matrix.o Polygon.o Window.o Color.o GLFunctions.o \
        Ellipse.o Rectangle.o Line.o Segment.o Circle.o Ring.o Disk.o Point.o \
        GLInitializer.o Mesh.o Shader.o Program.o Context.o Engine.o \
        BufferObject.o VertexArrayObject.o VertexData.o VertexAttribute.o \
        MeshManager.o MeshLoader.o Shape.o Renderer.o Engine.exe 
Matrix.o : $(src_util_math_path)Matrix.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Matrix.o \
    $(compiler_options_include) \
    $(src_util_math_path)Matrix.cpp"    
Shape.o : $(src_util_math_path)Shape.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Shape.o \
    $(compiler_options_include) \
    $(src_util_math_path)Shape.cpp"    
Common.o : $(src_util_math_path)Common.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Common.o \
    $(compiler_options_include) \
    $(src_util_math_path)Common.cpp"    
Vector.o : $(src_util_math_path)Vector.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Vector.o \
    $(compiler_options_include) \
    $(src_util_math_path)Vector.cpp"    
Point.o : $(src_util_math_path)Point.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Point.o \
    $(compiler_options_include) \
    $(src_util_math_path)Point.cpp"
Ellipse.o : $(src_util_math_path)Ellipse.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Ellipse.o \
    $(compiler_options_include) \
    $(src_util_math_path)Ellipse.cpp"    
Segment.o : $(src_util_math_path)Segment.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Segment.o \
    $(compiler_options_include) \
    $(src_util_math_path)Segment.cpp"    
Line.o : $(src_util_math_path)Line.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Line.o \
    $(compiler_options_include) \
    $(src_util_math_path)Line.cpp"    
Circle.o : $(src_util_math_path)Circle.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Circle.o \
    $(compiler_options_include) \
    $(src_util_math_path)Circle.cpp"    
Rectangle.o : $(src_util_math_path)Rectangle.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Rectangle.o \
    $(compiler_options_include) \
    $(src_util_math_path)Rectangle.cpp"    
Ring.o : $(src_util_math_path)Ring.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Ring.o \
    $(compiler_options_include) \
    $(src_util_math_path)Ring.cpp"   
Disk.o : $(src_util_math_path)Disk.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Disk.o \
    $(compiler_options_include) \
    $(src_util_math_path)Disk.cpp"   
Polygon.o : $(src_util_math_path)Polygon.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Polygon.o \
    $(compiler_options_include) \
    $(src_util_math_path)Polygon.cpp"   
Color.o : $(src_util_math_path)Color.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Color.o \
    $(compiler_options_include) \
    $(src_util_math_path)Color.cpp" 
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
Context.o : $(src_engine_graphics_util_path)Context.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Context.o \
    $(compiler_options_include) \
    $(src_engine_graphics_util_path)Context.cpp"   
MeshLoader.o : $(src_engine_graphics_util_path)MeshLoader.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)MeshLoader.o \
    $(compiler_options_include) \
    $(src_engine_graphics_util_path)MeshLoader.cpp"   
Engine.o : $(src_engine_graphics_core_path)Engine.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Engine.o \
    $(compiler_options_include) \
    $(src_engine_graphics_core_path)Engine.cpp"   
Renderer.o : $(src_engine_graphics_core_path)Renderer.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Renderer.o \
    $(compiler_options_include) \
    $(src_engine_graphics_core_path)Renderer.cpp"   
MeshManager.o : $(src_engine_graphics_core_path)MeshManager.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)MeshManager.o \
    $(compiler_options_include) \
    $(src_engine_graphics_core_path)MeshManager.cpp"   
GLFunctions.o : $(src_engine_graphics_gl_path)GLFunctions.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)GLFunctions.o \
    $(compiler_options_include) \
    $(src_engine_graphics_gl_path)GLFunctions.cpp"
GLInitializer.o : $(src_engine_graphics_gl_path)GLInitializer.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)GLInitializer.o \
    $(compiler_options_include) \
    $(src_engine_graphics_gl_path)GLInitializer.cpp"
Shader.o : $(src_engine_graphics_shader_path)Shader.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Shader.o \
    $(compiler_options_include) \
    $(src_engine_graphics_shader_path)Shader.cpp"
Program.o : $(src_engine_graphics_shader_path)Program.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Program.o \
    $(compiler_options_include) \
    $(src_engine_graphics_shader_path)Program.cpp"
Mesh.o : $(src_engine_graphics_drawing_path)Mesh.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)Mesh.o \
    $(compiler_options_include) \
    $(src_engine_graphics_drawing_path)Mesh.cpp"
VertexData.o : $(src_engine_graphics_drawing_path)VertexData.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)VertexData.o \
    $(compiler_options_include) \
    $(src_engine_graphics_drawing_path)VertexData.cpp"
VertexAttribute.o : $(src_engine_graphics_drawing_path)VertexAttribute.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)VertexAttribute.o \
    $(compiler_options_include) \
    $(src_engine_graphics_drawing_path)VertexAttribute.cpp"
BufferObject.o : $(src_engine_graphics_drawing_path)Mesh.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)BufferObject.o \
    $(compiler_options_include) \
    $(src_engine_graphics_drawing_path)BufferObject.cpp"
VertexArrayObject.o : $(src_engine_graphics_drawing_path)Mesh.cpp
    "$(compiler) \
    $(compiler_options) \
    $(compiler_options_out)VertexArrayObject.o \
    $(compiler_options_include) \
    $(src_engine_graphics_drawing_path)VertexArrayObject.cpp"
Test.exe : $(obj_files)
    "$(linker) \
    $(linker_options) \
    $(linker_options_out)Test.exe \
    $(obj_files) $(lib_files)"
Engine.exe : $(obj_files)
    "$(linker) \
    $(linker_options) \
    $(linker_options_out)Engine.exe \
    $(obj_files) $(lib_files)"
