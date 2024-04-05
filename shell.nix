# Shell expression for the Nix package manager
#
# This nix expression creates an environment with necessary packages installed:
#
#  * `tockloader`
#  * arm-none-eabi toolchain
#  * elf2tab
#  * riscv32-embedded toolchain
#
# To use:
#
#  $ nix-shell

{ pkgs ? import <nixpkgs-unstable> {
    config.allowUnfree = true;
  config.segger-jlink.acceptLicense = true;
}, disableRiscvToolchain ? false, withUnfreePkgs ? false }:

with builtins;
let
  inherit (pkgs) stdenv stdenvNoCC lib;

  tockloader = import (pkgs.fetchFromGitHub {
    owner = "tock";
    repo = "tockloader";
    rev = "v1.11.0";
    sha256 = "sha256-bPEfpfOZOjOiazqRgn1cnqe4ohLPvocuENKoZx/Qw80=";
  }) { inherit pkgs withUnfreePkgs; };
         allowUnfree = true;
         segger-jlink.acceptLicense = true;
        nixpkgs.config.allowUnfree = true;
         nixpkgs.config.segger-jlink.acceptLicense = true;
  elf2tab = pkgs.rustPlatform.buildRustPackage rec {
    name = "elf2tab-${version}";
    version = "0.12.0";

    src = pkgs.fetchFromGitHub {
      owner = "tock";
      repo = "elf2tab";
      rev = "v${version}";
      sha256 = "sha256-+VeWLBI6md399Oaumt4pJrOkm0Nz7fmpXN2TjglUE34=";
    };

    cargoSha256 = "sha256-UHAwk1fBcabRqy7VMhz4aoQuIur+MQshDOhC7KFyGm4=";
  };
in
  pkgs.mkShell {
    name = "tock-dev";

    buildInputs = with pkgs; [
      elf2tab
      gcc-arm-embedded
      python3Full
      tockloader
      uncrustify

      # ones specifically for developing openthread
      remake
      cmake
      cargo
    ] ++ (lib.optionals withUnfreePkgs [
      segger-jlink
      tockloader.nrf-command-line-tools
    ]) ++ (lib.optional (!disableRiscvToolchain) (
      pkgsCross.riscv32-embedded.buildPackages.gcc.override (oldCc: {
        cc = (pkgsCross.riscv32-embedded.buildPackages.gcc.cc.override (oldCcArgs: {
          libcCross = oldCcArgs.libcCross.overrideAttrs (oldNewlibAttrs: {
            configureFlags = oldNewlibAttrs.configureFlags ++ [
              "--enable-libssp"
            ];
          });
        })).overrideAttrs (oldCcAttrs: {
          configureFlags = oldCcAttrs.configureFlags ++ [
            "--without-headers"
            "--disable-shared"
            "--disable-libssp"
            "--enable-multilib"
            "--with-newlib"
          ];
          gcc_cv_libc_provides_ssp = "yes";
        });
      })
    ));

        permittedInsecurePackages = [
          "segger-jlink-qt4-794a"
        ];
              
    shellHook = ''
      ${if (!disableRiscvToolchain) then ''
        export RISCV=1
      '' else ""}

      # TODO: This should be patched into the rpath of the respective libraries!
      export LD_LIBRARY_PATH=${pkgs.libusb}/lib:${pkgs.segger-jlink}/lib:$LD_LIBRARY_PATH
    '';
  }

