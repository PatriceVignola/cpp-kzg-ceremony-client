# C++ KZG Ceremony Client

![Checks](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/actions/workflows/checks.yml/badge.svg)
![Windows CI](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/actions/workflows/windows-ci.yml/badge.svg)
![Darwin CI](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/actions/workflows/darwin-ci.yml/badge.svg)
![Ubuntu CI](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/actions/workflows/ubuntu-ci.yml/badge.svg)
![Alpine CI](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/actions/workflows/alpine-ci.yml/badge.svg)
![Debian CI](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/actions/workflows/debian-ci.yml/badge.svg)

The C++ KZG Ceremony Client (or CKCC) is a fully-featured client for Ethereum's KZG Ceremony.

## What is the KZG Ceremony?

The KZG Ceremony is a way for all Ethereum users to help scale Ethereum (i.e. EIP-4844, aka Proto-Danksharding) by adding to the credibility that nobody knows what the final secret is. In other words, the ceremony is secure as long as at least one honest person using one honest client (i.e. a client without bugs or undesired behavior) contributes. The more people contribute and the more diverse the set of clients is, higher are the chances that the ceremony is secure.

For more information, take a look at the [FAQ](https://github.com/ethereum/kzg-ceremony/blob/main/FAQ.md).

## Why C++?

Multiple clients for the Go, Rust and Java languages already exist, but I haven't seen a C++ client yet. For the sake of client and language diversity, I decided to build a C++ client. A diverse set of clients, languages and libraries adds to the robustness of the ceremony.

## Features

- Supports Windows, Mac and a wide range of Linux distributions from the last 10 years
- Supports Ethereum and GitHub accounts
- BLS signing
- ECDSA signing for Ethereum accounts
- Failure tolerant; while waiting for a contribution slot, the client will retry many times and even attempt to automatically re-authenticate when the session expires or when connection problems occur.
- Transcript verification
- Contribution verification
- Multiple layers of entropy for secret generation; in addition to the user's seed phrase, the secret generator will use the platform's CSPRNG (Cryptographically Secure PseudoRandom Number Generator) to improve entropy.
- All generated secrets in a contribution are unique
- Completely automated; while some clients need to the user to perform manual steps like opening their browser and copying some values back to the app, CKCC automatically opens the browser for each step and directs the user to the next step. The only manual step that the user needs to perform is to login through their GitHub or Ethereum account.
- Dual computer option that allows the powers to be computed on an airgapped machine disconnected from the internet, which means that secrets cannot be leaked

## Usage

1. Download the executable from the [releases page](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases)
2. Run the exeutable: `./cpp-kzg-ceremony-client`
3. Write your entropy sentence and press enter

More command-line options are also available:

```
  ______   __    __   ______    ______
 /      \ |  \  /  \ /      \  /      \
|  $$$$$$\| $$ /  $$|  $$$$$$\|  $$$$$$\
| $$   \$$| $$/  $$ | $$   \$$| $$   \$$
| $$      | $$  $$  | $$      | $$
| $$   __ | $$$$$\  | $$   __ | $$   __
| $$__/  \| $$ \$$\ | $$__/  \| $$__/  \
 \$$    $$| $$  \$$\ \$$    $$ \$$    $$
  \$$$$$$  \$$   \$$  \$$$$$$   \$$$$$$

C++ KZG Ceremony Client

Usage:
  ./build/bin/cpp-kzg-ceremony-client [OPTION...]

  -s, --sequencer arg           URL of the sequencer to use (default: 
                                https://seq.ceremony.ethereum.org)
  -a, --auth arg                Authentication provider to use. Choices: 
                                [ethereum, github] (default: ethereum)
  -m, --mode arg                Mode to use. `complete` mode goes through 
                                the whole process on a single machine. 
                                `internet` mode downloads the transcript 
                                from and submits the contribution to the 
                                sequencer, but doesn't generate any secrets 
                                or sign anything. `airgapped` mode is meant 
                                to be used on a computer without internet 
                                connection and takes a `contribution.json` 
                                file generated by a machine in `internet` 
                                mode, computes the powers of tau and 
                                updates the `contribution.json` file. 
                                Choices: [complete, internet, airgapped] 
                                (default: complete)
  -c, --contribution-file-path arg
                                Path to the contribution.json file used for 
                                the `internet` and `airgapped` modes.
  -e, --entropy arg             Type of entropy to use for the first layer. 
                                Additional CSPRNG entropy will be applied 
                                on top of it. Choices: [stdin] (default: 
                                stdin)
  -n, --no-signing              Disable the signing of the contribution. 
                                Although signing contributions is not 
                                mandatory, it is recommended to verify that 
                                the contributions listed in the transcript 
                                are actually generated by the addresses 
                                listed.
  -p, --port arg                Port to run the authentication server on. 
                                If not provided, a random port will be 
                                chosen.
  -h, --help                    Print usage
```

## Dual computer setup

Even when all precautions have been taken to not leak the secrets, a machine connected to the internet will always be somewhat vulnerable to attacks. To be 100% sure that your secrets will not be leaked, you can follow the dual computer setup:

1. On a computer connected to the internet, run the following command:

    `./cpp-kzg-ceremony-client --mode=internet --contribution-file-path=path/to/contribution.json`

2. When the program tells you to `move this file over to an airgapped machine`, copy the file to an USB drive.

3. Connect the USB drive to an airgapped machine and run the following command:

    `./cpp-kzg-ceremony-client --mode=airgapped --contribution-file-path=path/to/contribution.json`

4. When instructed, enter your entropy and, if using the Ethereum authentication, sign your contribution.

5. Finally, copy the file back to the internet-connected computer **at the same location that it was initially written to** and press `enter`. The program will now submit your contribution and your internet-connected computer will have no way to know what your secrets are!

Note that the same authentication provider has to be used on both the internet-connected machine and the airgapped machine.

## Latest Releases

| Platform          | Link                                                                                                                                                      |
| ----------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Windows           | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-windows-amd64.zip)           |
| MacOS             | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-darwin-amd64.tar.gz)         |
| Ubuntu 23.04      | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-ubuntu-23.04-amd64.tar.gz)   |
| Ubuntu 22.10      | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-ubuntu-22.10-amd64.tar.gz)   |
| Ubuntu 22.04      | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-ubuntu-22.04-amd64.tar.gz)   |
| Ubuntu 20.04      | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-ubuntu-20.04-amd64.tar.gz)   |
| Ubuntu 18.04      | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-ubuntu-18.04-amd64.tar.gz)   |
| Debian 11.6       | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-debian-11.6-amd64.tar.gz)    |
| Debian 10.13      | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-debian-10.13-amd64.tar.gz)   |
| Alpine Linux 3.17 | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.17.1-amd64.tar.gz)  |
| Alpine Linux 3.16 | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.16.3-amd64.tar.gz)  |
| Alpine Linux 3.15 | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.15.6-amd64.tar.gz)  |
| Alpine Linux 3.14 | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.14.8-amd64.tar.gz)  |
| Alpine Linux 3.13 | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.13.12-amd64.tar.gz) |
| Alpine Linux 3.12 | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.12.10-amd64.tar.gz) |
| Alpine Linux 3.11 | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.11.13-amd64.tar.gz) |
| Alpine Linux 3.10 | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.10.9-amd64.tar.gz)  |
| Alpine Linux 3.9  | [Download](https://github.com/PatriceVignola/cpp-kzg-ceremony-client/releases/download/v1.4.0/cpp-kzg-ceremony-client-v1.4.0-alpine-3.9.6-amd64.tar.gz)   |

## Dependencies

### Ubuntu

```
apt update && apt --yes upgrade
apt --yes install openssl
```

### Alpine Linux

```
apk --update add libstdc++
```

## Build from source

1. Clone the repo
2. Download CMake
3. Run `./build.sh` on Linux or `.\build.bat` on Windows

## Dependencies

- [jarro2783/cxxopts](https://github.com/jarro2783/cxxopts) for command-line arguments parsing
- [Corvusoft/restbed](https://github.com/Corvusoft/restbed) for the local HTTP server
- [libcpr/cpr](https://github.com/libcpr/cpr) for the HTTP client
- [nlohmann/json](https://github.com/nlohmann/json) for JSON serialization and deserialization
- [tristanpenman/valijson](https://github.com/tristanpenman/valijson) for JSON schema validation
- [supranational/blst](https://github.com/supranational/blst) for BLS signing and powers of tau functionality
- [Duthomhas/CSPRNG](https://github.com/Duthomhas/CSPRNG) for easy cross-platform CSPRNG
- [abseil/abseil-cpp](https://github.com/abseil/abseil-cpp) for helpers that allow C++14 programs to use modern C++ semantics and follow good practices
