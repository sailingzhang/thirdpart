rm *.h *.cc
./protoc  --grpc_out=. --plugin=protoc-gen-grpc=grpc_cpp_plugin.exe  *.proto
./protoc  --cpp_out=. *.proto

./protoc  -I  .  --go_out=plugins=grpc:.  *.proto

python -m grpc_tools.protoc --python_out=. --grpc_python_out=. -I. *.proto