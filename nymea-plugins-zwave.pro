TEMPLATE = subdirs

PLUGIN_DIRS = \
    zwavegeneric \
    zwavefibaro \
    zwavequbino \
    zwavespringswindowfashions \

message(============================================)
message("Qt version:" $$[QT_VERSION])

plugininfo.depends = FORCE
for (entry, PLUGIN_DIRS):plugininfo.commands += test -d $${entry} || mkdir -p $${entry}; cd $${entry} && qmake -o Makefile $$PWD/$${entry}/$${entry}.pro && cd ..;
for (entry, PLUGIN_DIRS):plugininfo.commands += make -C $${entry} plugininfo.h;
QMAKE_EXTRA_TARGETS += plugininfo

# Translations:
# make lupdate to update .ts files
lupdate.depends = FORCE plugininfo
for (entry, PLUGIN_DIRS):lupdate.commands += make -C $${entry} lupdate;
QMAKE_EXTRA_TARGETS += lupdate

# make lrelease to build .qm from .ts
lrelease.depends = FORCE
for (entry, PLUGIN_DIRS):lrelease.commands += lrelease $$files($$PWD/$${entry}/translations/*.ts, true);
QMAKE_EXTRA_TARGETS += lrelease

# For Qt-Creator's code model: Add CPATH to INCLUDEPATH explicitly
INCLUDEPATH += $$(CPATH)

message("Usage: qmake [srcdir] [WITH_PLUGINS=\"...\"] [WITHOUT_PLUGINS=\"...\"]")

isEmpty(WITH_PLUGINS) {
    PLUGINS = $${PLUGIN_DIRS}
} else {
    PLUGINS = $${WITH_PLUGINS}
}
PLUGINS-=$${WITHOUT_PLUGINS}

message("Building plugins:")
for(plugin, PLUGINS) {
    exists($${plugin}) {
        SUBDIRS*= $${plugin}
        message("- $${plugin}")
    } else {
        error("Invalid plugin \"$${plugin}\".")
    }
}
