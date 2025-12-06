{
	"targets": [
		{
			"target_name": "addon",
			"sources": [
				"src/addon.cpp",
				"src/bytecode/bytecode.cpp",
				"src/bytecode/prototype.cpp",
				"src/ast/ast.cpp",
				"src/lua/lua.cpp"
			],
			"include_dirs": [
				"<!@(node -p \"require('node-addon-api').include\")"
			],
			"defines": [
				"_CHAR_UNSIGNED",
				"NODE_ADDON",
				"NAPI_CPP_EXCEPTIONS"
			],
			"msvs_settings": {
				"VCCLCompilerTool": {
					"ExceptionHandling": 1,
					"AdditionalOptions": [
						"/J"
					]
				}
			}
		}
	]
}
