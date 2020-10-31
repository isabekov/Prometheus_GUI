Prometheus_Linux: Makefile.qmake_Linux
	make -f $<

Prometheus_MacOS: Makefile.qmake_MacOS
	make -f $<

Prometheus_MinGW32.exe: Makefile.qmake_MinGW32
	make -f $<

Prometheus_MinGW64.exe: Makefile.qmake_MinGW64
	make -f $<


Makefile.qmake_Linux: Prometheus_Linux.pro
	qmake -makefile -o $@ $<

Makefile.qmake_MacOS: Prometheus_MacOS.pro
	/opt/osxcross/bin/x86_64-apple-darwin15-qmake-qt5 -makefile -o $@ $<

Makefile.qmake_MinGW32: Prometheus_MinGW32.pro
	/opt/mxe/usr/bin/i686-w64-mingw32.static-qmake-qt5 -makefile -o $@ $<

Makefile.qmake_MinGW64: Prometheus_MinGW64.pro
	/opt/mxe/usr/bin/x86_64-w64-mingw32.static-qmake-qt5 -makefile -o $@ $<


Prometheus_Linux.pro:
	qmake -project -o $@
	echo "greaterThan(QT_MAJOR_VERSION, 4): QT += widgets" >> $@

Prometheus_MacOS.pro:
	/opt/osxcross/bin/x86_64-apple-darwin15-qmake-qt5 -project -o $@
	echo "CONFIG += release" >> $@
	echo "QMAKE_CXX   = x86_64-apple-darwin15-g++" >> $@
	echo "QMAKE_LINK  = x86_64-apple-darwin15-g++" >> $@
	echo "QMAKE_STRIP = x86_64-apple-darwin15-strip" >> $@
	echo "CONFIG += release" >> $@
	echo "ICON = Images/icon.icns" >> $@
	echo "QMAKE_TARGET_BUNDLE_PREFIX  = com.prometheus-fpga" >> $@
	echo "BitFiles.files = Resources/BitFiles" >> $@
	echo "BitFiles.path = Contents/Resources" >> $@
	echo "QMAKE_BUNDLE_DATA += BitFiles" >> $@
	echo "fireprog.files = Resources/fireprog" >> $@
	echo "fireprog.path = Contents/MacOS" >> $@
	echo "QMAKE_BUNDLE_DATA += fireprog" >> $@
	echo "greaterThan(QT_MAJOR_VERSION, 4.4): QT += widgets" >> $@

Prometheus_MinGW32.pro: Prometheus.rc Prometheus.manifest
	qmake -project -o $@
	echo "CONFIG += release" >> $@
	echo "RC_FILE = $<" >> $@
	echo "QMAKE_CXX   = /opt/mxe/usr/bin/i686-w64-mingw32.static-g++" >> $@
	echo "QMAKE_LINK  = /opt/mxe/usr/bin/i686-w64-mingw32.static-g++" >> $@
	echo "QMAKE_STRIP = /opt/mxe/usr/bin/i686-w64-mingw32.static-strip" >> $@
	echo "QMAKE_RC    = /opt/mxe/usr/bin/i686-w64-mingw32.static-windres --codepage=65001" >> $@
	echo "greaterThan(QT_MAJOR_VERSION, 4): QT += widgets" >> $@

Prometheus_MinGW64.pro: Prometheus.rc Prometheus.manifest
	qmake -project -o $@
	echo "CONFIG += release" >> $@
	echo "RC_FILE = $<" >> $@
	echo "QMAKE_CXX   = /opt/mxe/usr/bin/x86_64-w64-mingw32.static-g++" >> $@
	echo "QMAKE_LINK  = /opt/mxe/usr/bin/x86_64-w64-mingw32.static-g++" >> $@
	echo "QMAKE_STRIP = /opt/mxe/usr/bin/x86_64-w64-mingw32.static-strip" >> $@
	echo "QMAKE_RC    = /opt/mxe/usr/bin/x86_64-w64-mingw32.static-windres --codepage=65001" >> $@
	echo "greaterThan(QT_MAJOR_VERSION, 4): QT += widgets" >> $@

clean:
	rm -rf *.o release/ debug/ Prometheus_Linux Prometheus*.pro moc_MainWindow.cpp qrc_Resources.cpp moc_predefs.h ui_MainWindow.h Makefile.qmake_* .qmake.stash prometheus_plugin_import.cpp  prometheus_mingw32_plugin_import.cpp prometheus_macos_plugin_import.cpp
