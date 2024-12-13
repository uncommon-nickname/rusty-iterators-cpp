#!/bin/bash

clean_run=false
compile_tests=false
shared_lib_option="OFF"
cxx_compiler="g++"

while [[ $# -gt 0 ]]; do
	case $1 in
	--clang)
		cxx_compiler="clang++"
		shift
		;;
	--no-cache)
		clean_run=true
		shift
		;;
	--compile-tests)
		compile_tests=true
		shift
		;;
	--shared-lib)
		shared_lib_option="ON"
		shift
		;;
	*)
		echo "Unknown option $1"
		exit 1
		;;
	esac
done

echo
echo "Build configuration:"
echo "C++ Compiler        = ${cxx_compiler}"
echo "Clean Run           = ${clean_run}"
echo "Compile Tests       = ${compile_tests}"
echo "Compile Shared Lib  = ${shared_lib_option}"
echo

cmake \
	-G Ninja \
	-D CMAKE_CXX_COMPILER="${cxx_compiler}" \
	-D BUILD_SHARED_LIBS="${shared_lib_option}" \
	-D COMPILE_TESTS="${compile_tests}" \
	-S . \
	-B build

if [ "${clean_run}" = true ]; then
	echo -e "\nClearing the cache for fresh build...\n"
	cmake --build build --target clean
fi

echo -e "\nBuilding the project...\n"
cmake --build build
