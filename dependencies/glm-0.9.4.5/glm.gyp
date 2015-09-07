{
	'includes': [
		'../../build/common.gypi',
	],
	'targets': [
		{
			'target_name': 'glm',
			'type': 'none',
			'variables': {
				'project_dir': 'dependencies/glm-0.9.4.5',
			},
			'sources': [
				'glm/glm.hpp',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'.',
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