{
  cmake,
  extra-cmake-modules,
  lib,
  libgcc,
  libsForQt5,
  stdenv,
  ...
}:
stdenv.mkDerivation rec {
  pname = "fancontrol-gui";
  version = "v0.8";

  src = ../.;

  buildInputs = with libsForQt5; [
    kcmutils
    kdeclarative
    kio
    plasma-framework
    qt5.qtdeclarative
    qt5.qtquickcontrols
    qt5.qtquickcontrols2
  ];

  nativeBuildInputs = [
    cmake
    extra-cmake-modules
    libgcc
    libsForQt5.wrapQtAppsHook
  ];

  meta = with lib; {
    description = "A GUI for fancontrol with QT and KDE framework 5";
    homepage = "https://github.com/Maldela/fancontrol-gui";
    changelog = "https://github.com/Maldela/fancontrol-gui/releases/tag/${version}";
    license = licenses.gpl2;
    mainProgram = "fancontrol_gui";
  };
}
