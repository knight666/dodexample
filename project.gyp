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
				'TMPL_FEATURE_OPENGL_DEBUG=0'
			],
			'dependencies': [
				'dependencies/utf8rewind-1.1.1/utf8rewind.gyp:utf8rewind',
				'dependencies/freetype-2.5.0.1/freetype.gyp:freetype',
				'dependencies/glew-1.10.0/glew.gyp:glew',
				'dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'dependencies/glm-0.9.4.5/glm.gyp:glm',
			],
			'sources': [
				'source/Application.cpp',
				'source/Application.hpp',
				'source/Base.hpp',
				'source/Buffer.cpp',
				'source/Buffer.hpp',
				'source/FreeTypeLoader.cpp',
				'source/FreeTypeLoader.hpp',
				'source/main.cpp',
				'source/Quad.cpp',
				'source/Quad.hpp',
				'source/Program.cpp',
				'source/Program.hpp',
				'source/Shader.cpp',
				'source/Shader.hpp',
				'source/TextBatch.cpp',
				'source/TextBatch.hpp',
				'source/Texture.cpp',
				'source/Texture.hpp',
				'source/VertexArrays.cpp',
				'source/VertexArrays.hpp',
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