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
			"conditions": [
				["OS=='win'", {
					"msvs_settings": {
						"VCCLCompilerTool": {
							"ExceptionHandling": 1,
							"AdditionalOptions": [
								"/J"
							]
						},
						"VCLinkerTool": {
							"AdditionalOptions": [
								"/STACK:268435456"
							]
						}
					}
				}],
				["OS=='linux'", {
					"cflags": [
						"-funsigned-char",
						"-std=c++20",
						"-fexceptions"
					],
					"cflags_cc": [
						"-funsigned-char",
						"-std=c++20",
						"-fexceptions"
					],
					"ldflags": [
						"-Wl,-z,stack-size=268435456"
					]
				}]
			]
		}
	]
}
