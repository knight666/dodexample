{
	'includes': [
		'../../build/common.gypi',
	],
	'targets': [
		{
			'target_name': 'glfw',
			'type': 'static_library',
			'sources': [
				'include/GLFW/glfw3.h',
				'include/GLFW/glfw3native.h',
				'src/config.h',
				'src/clipboard.c',
				'src/context.c',
				'src/gamma.c',
				'src/init.c',
				'src/input.c',
				'src/internal.h',
				'src/joystick.c',
				'src/monitor.c',
				'src/time.c',
				'src/wgl_platform.h',
				'src/window.c',
				'src/win32_clipboard.c',
				'src/win32_gamma.c',
				'src/win32_init.c',
				'src/win32_joystick.c',
				'src/win32_monitor.c',
				'src/win32_platform.h',
				'src/win32_time.c',
				'src/win32_window.c',
				'src/wgl_context.c',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'include',
				],
			},
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
	],
}