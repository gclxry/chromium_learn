{
    'targets': [
      {
        'target_name': 'learn_base',
        'type': 'executable',
        'msvs_guid': '5ECEC9E5-8F23-47B6-93E0-C3B328B3BE65',
        'dependencies': [
          '../base/base.gyp:base',
        ],
        'include_dirs': [
          '..',
        ],
        'sources': [
        ],
		'msvs_settings': {
		  'VCLinkerTool': {
          'SubSystem': '2',  # Set /SUBSYSTEM:WINDOWS
          },
		},
      },
	  {
        'target_name': 'learn_base_console',
        'type': 'executable',
        'msvs_guid': '5ECEC9E5-8F23-47B6-93E0-C3B328B3BE65',
        'dependencies': [
          '../base/base.gyp:base',
        ],
        'include_dirs': [
          '..',
        ],
        'sources': [
          'pjj/learn_base_console/main.cc',
        ],
		'msvs_settings': {
		  'VCLinkerTool': {
          'SubSystem': '1',  # Set /SUBSYSTEM:CONSOLE
          },
		},
      },
    ],
}