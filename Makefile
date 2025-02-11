all:
	cmake ./ -B build
	make -C build

fast:
	@echo "won't redo the cmake file"
	make -C build

clean:
	rm -rf build
