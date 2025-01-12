#!/bin/bash

script_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
repo_root="${script_root%/tools}"

clean_run=false
compile_tests=false
cxx_compiler="g++"
compile_benchmarks=false
compile_examples=false
cmake_build_type="Debug"

while [[ $# -gt 0 ]]; do
	case $1 in
	--compiler)
		shift
		cxx_compiler=$1
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
		cmake_build_type="Release"
		shift
		;;
	--compile-examples)
		compile_examples=true
		shift
		;;
	--release)
		cmake_build_type="Release"
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
echo "Repository Root     = ${repo_root}"
echo "C++ Compiler        = ${cxx_compiler}"
echo "Clean Run           = ${clean_run}"
echo "Compile Tests       = ${compile_tests}"
echo "Compile Benchmarks  = ${compile_benchmarks}"
echo "Compile Examples    = ${compile_examples}"
echo "Cmake build type    = ${cmake_build_type}"
echo

cmake \
	-G Ninja \
	-D CMAKE_CXX_COMPILER="${cxx_compiler}" \
	-D COMPILE_TESTS="${compile_tests}" \
	-D COMPILE_BENCHMARKS="${compile_benchmarks}" \
	-D COMPILE_EXAMPLES="${compile_examples}" \
	-D CMAKE_BUILD_TYPE="${cmake_build_type}" \
	-S "${repo_root}" \
	-B build

if [ "${clean_run}" = true ]; then
	cmake --build build --target clean
fi
cmake --build build
