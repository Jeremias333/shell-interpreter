.PHONY: run clean run-dev clean-dev

./shell: ./src/main.c
	gcc $^ -o $@

./shell-dev: ./dev/main.c
	gcc $^ -o $@

run: 
	./shell

clean:
	rm shell

run-dev:
	./dev/shell

clean-dev:
	rm shell-dev

dev: ./shell-dev

build: ./shell