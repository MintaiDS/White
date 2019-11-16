set arg=%1
if "%arg%" equ "cmake" (
    call clean
    md Build
    md Build\Bin
    md Build\Obj
    cd Build 
    cmake ../Source 
    cd .. 
    call copy-executable.bat
) else if "%arg%" equ "msbuild" (
    cd Build
    msbuild White.sln
    cd ..
    call copy-executable.bat 
) else (
    call clean
    md Build
    md Build\Bin
    md Build\Obj
    nmake engine 
)
