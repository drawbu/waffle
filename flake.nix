{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-23.05";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils } @inputs:
    utils.lib.eachDefaultSystem (system:
      with import nixpkgs { inherit system; }; {
        devShells.default = mkShell {
          packages = [
            python311Packages.compiledb
            gcc12
            glibc
            ltrace
            gnumake
            man
            man-pages
            xorg.libX11
            xorg.libXft
            xorg.libXfont
            xorg.libXcursor
            mesa
            valgrind
          ];
        };
      });
}
