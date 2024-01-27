{
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem [ "x86_64-linux" ] (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      rec {
        devShell = with pkgs; mkShell {
          hardeningDisable = [ "all" ];
          inputsFrom = [ packages.waffle ];
          packages = [
            python311Packages.compiledb
            ltrace
            man
            man-pages
            mesa
            valgrind
          ];
        };

        formatter = pkgs.nixpkgs-fmt;

        packages = rec {
          default = waffle;
          waffle = pkgs.stdenv.mkDerivation rec {
            name = "waffle";
            src = ./.;

            hardeningDisable = [ "format" "fortify" ];
            buildInputs = with pkgs; let
              xlibraries = with xorg; [
                libX11
                libXft
                libXfont
                libXcursor
              ];
            in
            [ gnumake ] ++ xlibraries;

            installPhase = ''
              runHook preInstall

              mkdir -p $out/bin
              cp ${name} $out/bin/${name}

              runHook postInstall
            '';
          };
        };
      });
}
