rem This is for if you are using dynamic shader compilation, and just want the .incs to build.

set dynamic_shaders=1
call buildbs5shaders.bat
set dynamic_shaders=
