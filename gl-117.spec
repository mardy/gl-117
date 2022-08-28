%define name gl-117
%define version 0.8.8

Summary: An OpenGL and SDL based action flight simulator
Summary(de): Ein OpenGL- und SDL-basierter Flugsimulator
Summary(pl): Zrêczno¶ciowy symulator lotu u¿ywaj±cy OpenGL i SDL
Name: %{name}
Version: %{version}
Release: 1
Copyright: GPL
Vendor: Thomas A. Drexl <tom.drexl@gmx.de>
Url: http://home.t-online.de/home/Primetime./
Packager: Dominik Seichter <domseichter@web.de>
Group: x11/games
Source: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-%{version}

%description
GL-117 is an OpenGL and SDL-based action flight simulator written in
C++. It provides a random terrain generator, lighting effects, sounds,
and joystick support. Predefined levels of video quality and an amount
of viewing ranges let you perfectly adjust the game to the performance
of your system.

%description -l de
GL-117 ist ein in C++ entwickelter, OpenGL- und SDL-basierter
Action-Flugsimulator. Neben per Zufall erzeugten Landschaften besticht
das Spiel durch Lichteffekte, Sound und Joystickunterstützung. Anhand
vordefinierter Qualitätsstufen lässt sich die Grafik optimal an die
Leistung des eigenen Systems anpassen.

%description -l pl
GL-117 jest zrêczno¶ciowym symulatorem lotu napisanym w C++,
wykorzystuj±cym biblioteki OpenGL i SDL. Posiada obs³ugê joysticka,
d¼wiêk, losowo generowany teren, efekty ¶wietlne. Predefiniowane
poziomy jako¶ci obrazu oraz szeroki zakres ustawieñ widoczno¶ci
pozwalaj± idealnie dopasowaæ grê do wydajno¶ci systemu.

%prep
%setup
./configure

%build

# Setup for parallel builds
numprocs=`egrep -c ^cpu[0-9]+ /proc/stat || :`
if [ "$numprocs" = "0" ]; then
  numprocs=1
fi

make -j$numprocs

%install
make install-strip DESTDIR=$RPM_BUILD_ROOT

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > $RPM_BUILD_DIR/file.list.gl-117
find . -type f | sed 's,^\.,\%attr(-\,root\,root) ,' >>  $RPM_BUILD_DIR/file.list.gl-117
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >>  $RPM_BUILD_DIR/file.list.gl-117

%clean
rm -rf $RPM_BUILD_ROOT/*
rm -rf $RPM_BUILD_DIR/gl-117
rm -rf ../file.list.gl-117

%files -f ../file.list.gl-117



