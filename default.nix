with import <nixpkgs> {}; rec {
  COSEC = stdenv.mkDerivation {
    name = "COSEC";
    buildInputs = [
      which
      xorriso # for res/mkcd
      qemu
    ];
  };
}
