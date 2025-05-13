{
  description = "A GUI for fancontrol with QT and KDE framework 5";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts = {
      url = "github:hercules-ci/flake-parts";
      inputs.nixpkgs-lib.follows = "nixpkgs";
    };
  };

  outputs = inputs @ {
    self,
    flake-parts,
    ...
  }:
    flake-parts.lib.mkFlake {inherit inputs;} {
      systems = ["x86_64-linux" "aarch64-linux"];

      perSystem = {
        self',
        pkgs,
        system,
        ...
      }: {
        _module.args.pkgs = import self.inputs.nixpkgs {
          inherit system;
        };
        devShells.default = pkgs.mkShell {
          inputsFrom = builtins.attrValues self'.packages;
          packages = with pkgs; [
            git
            libgcc
            cmake
            extra-cmake-modules
            gettext
            libsForQt5.kio
            libsForQt5.qt5.qtquickcontrols
            libsForQt5.qt5.qtquickcontrols2
            libsForQt5.qt5.qtdeclarative
            libsForQt5.kdeclarative
            libsForQt5.kcmutils
            libsForQt5.plasma-framework
          ];
        };
        packages = rec {
          fancontrol-gui = pkgs.callPackage ./nix/default.nix {stdenv = pkgs.stdenv;};
          default = fancontrol-gui;
        };
      };
      flake = _: rec {
        nixosModules.home-manager = homeManagerModules.default;
        homeManagerModules = rec {
          fancontrol-gui = import ./nix/hm.nix inputs.self;
          default = fancontrol-gui;
        };
      };
    };
}
