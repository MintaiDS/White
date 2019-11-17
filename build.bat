set arg=%1
call clean
md Build
md Build\Bin
md Build\Obj
cd Build 
cmake ../Source 
cd .. 
copy Build\Engine\Graphics\Core\Debug\Engine.exe Build\Bin\
cd Build
msbuild White.sln
cd ..
copy Build\Engine\Graphics\Core\Debug\Engine.exe Build\Bin\
xcopy /e External\WGForgeTestData\src\WGForgeTestData
ren Test test_graphs
