{
	'includes': [
		'build/common.gypi',
	],
	'targets': [
		{
			'target_name': 'DODExample',
			'type': 'executable',
			'defines': [
				'TMPL_WINDOW_TITLE="DODExample"',
				'TMPL_WINDOW_WIDTH=1280',
				'TMPL_WINDOW_HEIGHT=720',
				'TMPL_FEATURE_OPENGL_DEBUG=1'
			],
			'dependencies': [
				'dependencies/utf8rewind-1.1.1/utf8rewind.gyp:utf8rewind',
				'dependencies/freetype-2.5.0.1/freetype.gyp:freetype',
				'dependencies/glew-1.10.0/glew.gyp:glew',
				'dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'dependencies/glm-0.9.4.5/glm.gyp:glm',
			],
			'include_dirs': [
				'source'
			],
			'sources': [
				'source/base/Main.hpp',
				'source/base/Utils.cpp',
				'source/base/Utils.hpp',
				'source/logging/Context.cpp',
				'source/logging/Context.hpp',
				'source/logging/Levels.hpp',
				'source/logging/Logger.cpp',
				'source/logging/Logger.hpp',
				'source/logging/Sink.hpp',
				'source/logging/SinkConsole.cpp',
				'source/logging/SinkConsole.hpp',
				'source/logging/SinkFile.cpp',
				'source/logging/SinkFile.hpp',
				'source/graphics/Buffer.cpp',
				'source/graphics/Buffer.hpp',
				'source/graphics/Quad.cpp',
				'source/graphics/Quad.hpp',
				'source/graphics/Program.cpp',
				'source/graphics/Program.hpp',
				'source/graphics/Shader.cpp',
				'source/graphics/Shader.hpp',
				'source/graphics/Texture.cpp',
				'source/graphics/Texture.hpp',
				'source/graphics/VertexArrays.cpp',
				'source/graphics/VertexArrays.hpp',
				'source/text/FreeTypeLoader.cpp',
				'source/text/FreeTypeLoader.hpp',
				'source/text/TextBatch.cpp',
				'source/text/TextBatch.hpp',
				'source/Application.cpp',
				'source/Application.hpp',
				'source/main.cpp',
			],
			'libraries': [
				'-lkernel32',
				'-luser32',
				'-lgdi32',
				'-lshell32',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}],
			],
		},
	],
}