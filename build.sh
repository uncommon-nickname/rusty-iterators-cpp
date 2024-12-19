#!/bin/bash

clean_run=false
compile_tests=false
cxx_compiler="g++"
compile_benchmarks=false

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
	--compile-benchmarks)
		compile_benchmarks=true
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
echo "Compile Benchmarks  = ${compile_benchmarks}"
echo

cmake \
	-G Ninja \
	-D CMAKE_CXX_COMPILER="${cxx_compiler}" \
	-D COMPILE_TESTS="${compile_tests}" \
	-D COMPILE_BENCHMARKS="${compile_benchmarks}" \
	-S . \
	-B build

if [ "${clean_run}" = true ]; then
	echo -e "\nClearing the cache for fresh build...\n"
	cmake --build build --target clean
fi

echo -e "\nBuilding the project...\n"
cmake --build build
