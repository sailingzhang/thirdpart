rm *.h *.c
bindir="../../grpc1.27.1/bin"
${bindir}/protoc  --grpc_out=. --plugin=protoc-gen-grpc=${bindir}/grpc_cpp_plugin  *.proto
${bindir}/protoc  --cpp_out=. *.proto
${bindir}/protoc  -I  .  --go_out=plugins=grpc:.  *.proto

python -m grpc_tools.protoc --python_out=. --grpc_python_out=. -I. *.proto
