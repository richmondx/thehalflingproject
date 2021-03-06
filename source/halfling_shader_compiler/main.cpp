/* The Halfling Project - A Graphics Engine and Projects
 *
 * The Halfling Project is the legal property of Adrian Astley
 * Copyright Adrian Astley 2013 - 2014
 */

#include "halfling_shader_compiler/halfling_shader_compiler.h"

#include <iostream>
#include <filesystem>


int main(int argc, char *argv[]) {
	// Check the number of parameters
	if (argc < 3) {
		// Tell the user how to run the program
		std::cerr << "Usage: hsc.exe <outputDirectory> <json filePath>" << std::endl;
		return 1;
	}

	std::tr2::sys::path outputDirectory(argv[1]);
	std::tr2::sys::path jsonFilePath(argv[2]);

	return HalflingShaderCompiler::CompileFiles(outputDirectory, jsonFilePath);
}