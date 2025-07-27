function(UseCppCheck target sources headers)
	 find_program(CPPCHECK_COMMAND cppcheck) 
	 if (CPPCHECK_COMMAND) 
		 message(STATUS "Use CppCheck for target: ${target}")	
		 add_custom_command(TARGET ${target}
		 					POST_BUILD  
		 					COMMAND ${CPPCHECK_COMMAND} --output-file=${CMAKE_SOURCE_DIR}/cppcheck_report.xml --output-format=xml --checkers-report=${CMAKE_SOURCE_DIR}/cpp_check.xml --cppcheck-build-dir=${CMAKE_BINARY_DIR} -I ${QSL_INCLUDE_DIR}/QSL -j4 --language=c++ --std=c++${CMAKE_CXX_STANDARD} --enable=information,performance,warning,portability,unusedFunction -D__cplusplus=202002 -D_WIN64 -D_MSC_VER=202002 --platform=win64 ${${sources}} ${${headers}} ) 
	endif() 
endfunction()
