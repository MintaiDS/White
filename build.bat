cmake -E remove_directory Build
cmake -E make_directory Build
cmake -E make_directory Build/Bin
cmake -E make_directory Build/Obj
cd Build
cmake ../Source 
cd .. 
cmake -E copy Build/Engine/Graphics/Core/Debug/Engine.exe Build/Bin/
cmake --build Build
cmake -E copy Build/Engine/Graphics/Core/Debug/Engine.exe Build/Bin/
cmake -E make_directory test_graphs
cmake -E copy_directory External/WGForgeTestData/src/WGForgeTestData/Test ./Test/
cmake -E copy External/WGForgeTestData/src/WGForgeTestData/test.bat ./
