#!/bin/bash
# marc/os - NEM Software

branch=$1

function help {
	cat << EOF
This rep include copies of docs sourced elsewhere.
Usage:
	$0 <branch>
EOF
}

if [[ "_${branch}" == "_" ]]; then
	help
	exit 1
fi

prj="symbol-sdk-core-cpp"
gitUrl="https://github.com/nemtech/${prj}.git"
if [[ "_$branch" == "_" ]]; then
	branch="dev"
fi

file="FindSYMBOLCORE.cmake"
echo "Obtaining the ${file} from branch ${branch} in ${gitUrl}"
rm -rf ${prj}
git clone -b ${branch} ${gitUrl}
cp ${prj}/cmake/Modules/FindSYMBOLCORE.cmake cmake/Modules/

git add cmake/Modules/FindSYMBOLCORE.cmake
git commit -m "build: Automated syncing files published elsewhere

cmakemodules Find*.cmake"

