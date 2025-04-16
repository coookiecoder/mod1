all:
	@cmake ./ -B build -DCMAKE_BUILD_TYPE=Release
	@make -C build

fast:
	@echo "won't redo the cmake file"
	@make -C build

clean:
	@rm -rf build
