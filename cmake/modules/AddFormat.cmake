set(stone_include_files
	${STONE_SOURCE_DIR}/include/stone/Frontend/*.h
)
set(stone_lib_files
	${STONE_SOURCE_DIR}/lib/AST/*.cpp
)
set(stone_tools_files
	${STONE_SOURCE_DIR}/tools/frontend/*.cpp  
	${STONE_SOURCE_DIR}/tools/driver/*.cpp  
)
set(stone_tests_files
	${STONE_SOURCE_DIR}/tests/units/Basic/*.cpp 
)
find_program(CLANG_FORMAT clang-format)
	if(CLANG_FORMAT)
	add_custom_target(
		stone-format
		clang-format
		-i
		-style=llvm
		${stone_lib_files}
		${stone_include_files}
	)
endif()
