@echo off

@setlocal
pushd "%~dp0"

set path=%cd%
set output_path=%path%\msg
set protoc_path=D:\Others\Dependencies\protobuf\protobuf-3.20.3\bin\windows
set grpc_protoc_plugins_path=D:\Others\Dependencies\grpc\v1.51.3\bin\Windows

%protoc_path%\protoc.exe message.proto -I %output_path%\message -I %output_path%\plugins -I %protoc_path%\..\..\src --cpp_out=%output_path%\ --proto_path=%output_path%\message
%protoc_path%\protoc.exe plc.proto -I %output_path%\message -I %output_path%\plugins -I %protoc_path%\..\..\src --cpp_out=%output_path%\ --proto_path=%output_path%\plugins

rem %protoc_path%\protoc.exe helloworld.proto --grpc_out=%output_path%\ --plugin=protoc-gen-grpc=%grpc_protoc_plugins_path%\grpc_cpp_plugin.exe --proto_path=.\protos\


popd
@endlocal