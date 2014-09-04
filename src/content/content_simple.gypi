# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'content_simple_product_name': 'Content simple',
    # The "19" is so that sites that sniff for version think that this is
    # something reasonably current; the "77.34.5" is a hint that this isn't a
    # standard Chrome.
    'content_simple_version': '1.0.0.0',
  },
  'targets': [

    {
      'target_name': 'content_simple',
      'type': 'executable',
      'defines!': ['CONTENT_IMPLEMENTATION'],
      'variables': {
        'chromium_code': 1,
      },
      'dependencies': [
	    'browser/devtools/devtools_resources.gyp:devtools_resources',
		'<(DEPTH)/ui/ui.gyp:ui_resources',
        '../third_party/mesa/mesa.gyp:osmesa',
		'content_app',
        'content_browser',
        'content_common',
        'content_gpu',
        'content_plugin',
        'content_ppapi_plugin',
        'content_renderer',
        'content_shell_resources',
        'content_utility',
        'content_worker',
        'test_support_content',
        'content_resources.gyp:content_resources',
        '../base/base.gyp:base',
        '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../build/temp_gyp/googleurl.gyp:googleurl',
        '../ipc/ipc.gyp:ipc',
        '../media/media.gyp:media',
        '../net/net.gyp:net',
        '../net/net.gyp:net_resources',
        '../skia/skia.gyp:skia',
        '../ui/gl/gl.gyp:gl',
        '../ui/ui.gyp:ui',
        '../v8/tools/gyp/v8.gyp:v8',
        '../webkit/support/webkit_support.gyp:webkit_resources',
        '../webkit/support/webkit_support.gyp:webkit_support',
        '<(webkit_src_dir)/Source/WebKit/chromium/WebKit.gyp:webkit',
        '<(webkit_src_dir)/Source/WebKit/chromium/WebKit.gyp:webkit_test_support',
        '<(webkit_src_dir)/Tools/DumpRenderTree/DumpRenderTree.gyp/DumpRenderTree.gyp:TestRunner',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
        'app/startup_helper_win.cc',
        'simple/AboutDlg.cpp',
		'simple/AboutDlg.h',
		'simple/AddressBar.cpp',
		'simple/AddressBar.h',
		'simple/MainFrm.cpp',
		'simple/MainFrm.h',
		'simple/resource.h',
		'simple/SimpleClient.cpp',
		'simple/SimpleClient.h',
		'simple/SimpleTab.cpp',
		'simple/SimpleTab.h',
		'simple/Simple.cpp',
		'simple/Simple.h',
		'simple/Simple.rc',
		'simple/stdafx.cpp',
		'simple/stdafx.h',
		'simple/simple_browser_context.cc',
		'simple/simple_browser_context.h',
		'simple/simple_browser_main_parts.cc',
		'simple/simple_browser_main_parts.h',
		'simple/simple_content_browser_client.cc',
		'simple/simple_content_browser_client.h',
		'simple/simple_content_client.cc',
		'simple/simple_content_client.h',
		'simple/simple_main_delegate.cc',
		'simple/simple_main_delegate.h',
		'simple/simple_url_request_context_getter.cc',
		'simple/simple_url_request_context_getter.h',
		'simple/simple_web_contents_delegate.cpp',
		'simple/simple_web_contents_delegate.h',
      ],
      'msvs_settings': {
        'VCLinkerTool': {
          'SubSystem': '2',  # Set /SUBSYSTEM:WINDOWS
        },
      },
      'conditions': [
        ['OS=="win" and win_use_allocator_shim==1', {
          'dependencies': [
            '../base/allocator/allocator.gyp:allocator',
          ],
        }],
        ['OS=="win"', {
		  'resource_include_dirs': [
            '<(SHARED_INTERMEDIATE_DIR)/webkit',
          ],
		  'dependencies': [
            '<(DEPTH)/webkit/support/webkit_support.gyp:webkit_strings',
          ],
          'sources': [
           # 'shell/shell.rc',
          ],
          'configurations': {
            'Debug_Base': {
              'msvs_settings': {
                'VCLinkerTool': {
                  'LinkIncremental': '<(msvs_large_module_debug_link_mode)',
                },
              },
            },
          },
		  'msvs_disabled_warnings': [ 4267, ],
        }],  # OS=="win"
        ['OS == "win" or (toolkit_uses_gtk == 1 and selinux == 0)', {
          'dependencies': [
            '../sandbox/sandbox.gyp:sandbox',
          ],
        }],  # OS=="win" or (toolkit_uses_gtk == 1 and selinux == 0)
        ['toolkit_uses_gtk == 1', {
          'dependencies': [
            '<(DEPTH)/build/linux/system.gyp:gtk',
          ],
        }],  # toolkit_uses_gtk
      ],
    },
  ],
  'conditions': [
  ]
}
