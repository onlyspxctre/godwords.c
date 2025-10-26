all: clean build

name := godwords

build:
	clang -Wall -Wextra -std=c99 -pedantic ${name}.c -o ${name} -fsanitize=address

clean:
	rm -f ./${name}
