project('beanbattery', 'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

bbcli = [
  'src/bbcli/main.cc'
]

bbgui = [
  'src/bbgui/main.cc'
]
exe = executable('bbcli', bbcli, install : true)

exe = executable('bbgui', bbgui, install : true)

test('basic', exe)
