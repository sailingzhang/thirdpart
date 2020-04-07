rm *.h *.cc *.js *.ts *.py *.go
bindir="../../grpc1.27.1/bin"
${bindir}/protoc  --grpc_out=. --plugin=protoc-gen-grpc=${bindir}/grpc_cpp_plugin  *.proto
${bindir}/protoc  --cpp_out=. *.proto
${bindir}/protoc  -I  .  --go_out=plugins=grpc:.  *.proto

python -m grpc_tools.protoc --python_out=. --grpc_python_out=. -I. *.proto

${bindir}/protoc -I=. *.proto   --plugin=protoc-gen-grpc-web=${bindir}/protoc-gen-grpc-web  --js_out=import_style=commonjs,binary:. --grpc-web_out=import_style=typescript,mode=grpcwebtext:.
