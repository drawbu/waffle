{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-22.11";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils } @inputs:
    utils.lib.eachDefaultSystem (system:
      with import nixpkgs { inherit system; }; {
        devShells.default = mkShell {
          packages = [
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
