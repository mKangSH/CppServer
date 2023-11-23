protoc.exe -I=./ --cpp_out=./ --csharp_out=./ ./Protocol.proto
IF ERRORLEVEL 1 PAUSE