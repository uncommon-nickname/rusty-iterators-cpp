#!/bin/bash

cxx_files_list="$(cd "${repo_root}" && git ls-tree --full-tree --name-only -r HEAD | grep -E ".*(\.cpp|\.hpp)")"
clang-format --verbose -i $cxx_files_list
