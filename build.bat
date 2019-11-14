set arg=%1
if "%arg%" equ "cmake" (
    call clean
    md Build
    md Build\Bin
    md Build\Obj
    cd Build 
    cmake ../Source 
    cd .. 
    copy Build\Engine\Render\Core\Debug\Engine.exe Build\Bin\
) else if "%arg%" equ "msbuild" (
    cd Build
    msbuild White.sln
    copy Engine\Render\Core\Debug\Engine.exe Bin\
    cd ..
) else (
    call clean
    md Build
    md Build\Bin
    md Build\Obj
    nmake engine 
)
exit
