self: {
  config,
  pkgs,
  lib,
  ...
}: let
  cfg = config.programs.fancontrol-gui;
  defaultPackage = self.packages.${pkgs.stdenv.hostPlatform.system}.default;
in {
  options.programs.fancontrol-gui = with lib; {
    enable = mkEnableOption "fancontrol-gui";

    package = mkOption {
      type = with types; nullOr package;
      default = defaultPackage;
      defaultText = lib.literalExpression ''
        fancontrol-gui.packages.''${pkgs.stdenv.hostPlatform.system}.default
      '';
      description = mdDoc ''
        Package to run
      '';
    };
  };
  config = lib.mkIf cfg.enable {
    home.packages = lib.optional (cfg.package != null) cfg.package;
  };
}
