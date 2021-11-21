#!/bin/bash

# Use first arg as build directory, use 'build' otheriwse.
buildDir=./build
if [ $# = 1 ] && [ "$1" != "" ] 
then
    buildDir="$1"
fi

# Create the build directory if it doesn't already exist.
[[ -d "$buildDir" ]] || mkdir "$buildDir"

# Loop over all .test.h files in the current directory.
for testClassFile in *.test.h
do
    # Grab the name of the test class and parse the built C filename 
    # with the build path.
    testClassName=$( echo "$testClassFile" | grep -o "^[^.]*" )
    testClassBuild="$buildDir"/"$testClassName".test.cpp

    echo Building \'"$testClassName"\'...

    # Make an empty build file and populate it with some boiler plate code.
    >| "$testClassBuild"
    echo -e "#include <stdio.h>\n#include \"tests/$testClassFile\"\n" >> "$testClassBuild"
    echo -e "int main() {\n\t$testClassName testClass;" >> "$testClassBuild"

    # Call each test case in the class in main.
    for testCase in $( cat "$testClassFile" | grep -v "[[:space:]]*//" | \
        sed -nr "s/[[:space:]]*void (.*) \{/\1/p" )
    do
        echo -e "\tprintf(\"Executing $testCase\\\\n\");\n\ttestClass.$testCase;" >> "$testClassBuild"
    done
    echo -e "\n\tprintf(\"Done.\\\\n\");\n\treturn 0;\n}" >> "$testClassBuild"

    echo Successfully built to "$testClassBuild".
done
