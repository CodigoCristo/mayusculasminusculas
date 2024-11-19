

resources = gnome.compile_resources(
    'resources',
    'gresource.xml',
    source_dir: meson.current_build_dir(),
)

datadir = get_option('datadir')

# Instala el archivo .desktop en el sistema para el menú de aplicaciones
install_data(
    'MayusApp.desktop',
    install_dir: join_paths(datadir, 'applications'),
    rename: meson.project_name() + '.desktop'
)

# Instala el archivo .appdata.xml para AppCenter o similares
install_data(
    'MayusApp.appdata.xml',
    install_dir: join_paths(datadir, 'metainfo'),
    rename: meson.project_name() + '.appdata.xml'
)

# Install our icons in all the required sizes
icon_sizes = ['16', '24', '32', '48', '64', '128']
foreach i : icon_sizes
    install_data(
        'icons' / i + '.svg',
        install_dir: get_option('datadir') / 'icons' / 'hicolor' / i + 'x' + i / 'apps',
        rename: meson.project_name() + '.svg'
    )
    install_data(
        'icons' / i + '.svg',
        install_dir: get_option('datadir') / 'icons' / 'hicolor' / i + 'x' + i + '@2' / 'apps',
        rename: meson.project_name() + '.svg'
    )
endforeach
