mkdir build
pushd build
gcc -Wall -Wextra -pedantic -o snake ../src/game.c ../src/terminal.c  ../src/snake.c
popd
