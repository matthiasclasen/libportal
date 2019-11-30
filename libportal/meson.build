headers = files('portal.h',
                'portal-gtk.h',
                'portal-helpers.h',
                'account.h',
                'background.h',
                'camera.h',
                'email.h',
                'filechooser.h',
                'inhibit.h',
                'location.h',
                'notification.h',
                'openuri.h',
                'print.h',
                'remote.h',
                'screenshot.h',
                'spawn.h',
                'trash.h',
                'updates.h')

portal_enums = gnome.mkenums('portal-enums',
                             sources: headers,
                             c_template: 'portal-enums.c.template',
                             h_template: 'portal-enums.h.template',
                             install_dir: join_paths (get_option('prefix'), 'include', 'libportal'),
                             install_header: true)

src = [
	'account.c',
        'background.c',
        'camera.c',
	'email.c',
        'filechooser.c',
	'inhibit.c',
        'location.c',
	'notification.c',
	'openuri.c',
	'portal.c',
        'portal-gtk.c',
        'print.c',
        'remote.c',
	'screenshot.c',
        'session.c',
        'spawn.c',
        'trash.c',
        'updates.c',
	'utils.c',
        portal_enums
]

gio_dep = dependency('gio-2.0')
gio_unix_dep = dependency('gio-unix-2.0')
gtk_dep = dependency('gtk+-3.0')

install_headers(headers, subdir: 'libportal')

libportal = library('portal',
                    src,
                    version: '0.0.1',
                    soversion: 0,
                    include_directories: [top_inc, libportal_inc],
                    c_args: visibility_args,
                    install: true,
                    dependencies: [gio_dep, gio_unix_dep, gtk_dep])

libportal_dep = declare_dependency(sources: src,
                                   include_directories: top_inc,
                                   dependencies: [gio_dep, gio_unix_dep],
                                   link_with: libportal)

if get_option('gir')
  libportal_gir = gnome.generate_gir(
      libportal,
      sources: src + headers + ['docs.c'],
      header: 'portal-gtk.h',
      nsversion: '1.0',
      namespace: 'Xdp',
      symbol_prefix: 'xdp',
      identifier_prefix: 'Xdp',
      includes: [
          'GObject-2.0',
          'Gio-2.0',
          'Gtk-3.0',
      ],
      dependencies: [libportal_dep],
      install: true,
      build_by_default: true,
  )
  if get_option('vapi')
    libportal_vala_dep = gnome.generate_vapi('libportal',
                                             sources: libportal_gir[0],
                                             packages: [
                                                 'gio-2.0',
                                                 'gtk+-3.0',
                                             ],
                                             install: true,
    )
  endif
else
  if get_option('vapi')
    error('Can\'t generate VAPI without GIR')
  endif
endif
