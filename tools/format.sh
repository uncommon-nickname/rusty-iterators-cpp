#!/bin/bash

script_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
repo_root="${script_root%/tools}"

dry_run=false

while [[ $# -gt 0 ]]; do
	case $1 in
	--dry-run)
		dry_run=true
		shift
		;;
	*)
		echo "Unknown option $1"
		exit 1
		;;
	esac
done

cxx_files_list="$(cd "${repo_root}" && git ls-tree --full-tree --name-only -r HEAD | grep -E ".*(\.cpp|\.hpp)")"

if [ "${dry_run}" = true ]; then
    (cd "${repo_root}" && clang-format --dry-run --verbose -i $cxx_files_list)
else
    (cd "${repo_root}" && clang-format --verbose -i $cxx_files_list)
fi
