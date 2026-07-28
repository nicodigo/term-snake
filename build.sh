mkdir build
pushd build
gcc -Wall -Wextra -pedantic -o snake ../src/snake.c ../src/terminal.c
popd
