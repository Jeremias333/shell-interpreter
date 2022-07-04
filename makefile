.PHONY: run clean run-dev clean-dev

./build: ./src/main.c
	gcc $^ -o $@

./build-dev: ./dev/main.c
	gcc $^ -o $@

run: 
	./build

clean:
	rm build

run-dev:
	./dev/build

clean-dev:
	rm ./dev/build

dev: ./build-dev

build: ./build