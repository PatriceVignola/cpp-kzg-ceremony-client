@echo off

set GREEN=[32m
set NO_COLOR=[0m

rem Gather all cmake files in a list
set CMAKE_FILES=
for %%v in (*.cmake) do set CMAKE_FILES=%CMAKE_FILES% "%%v"
for %%v in (*CMakeLists.txt) do set CMAKE_FILES=%CMAKE_FILES% "%%v"

for /D %%D in (".\*.*") do @(
    if /I not "%%~nxD"=="build" (
        pushd "%%~D"
        for /r %%v in (*.cmake) do set CMAKE_FILES=%CMAKE_FILES% "%%v"
        for /r %%v in (*CMakeLists.txt) do set CMAKE_FILES=%CMAKE_FILES% "%%v"
        popd
    )
)

rem Gather all C++ files in a list
set CPP_FILES=
for %%v in (*.h) do set CPP_FILES=%CPP_FILES% "%%v"
for %%v in (*.cc) do set CPP_FILES=%CPP_FILES% "%%v"

for /D %%D in (".\*.*") do @(
    if /I not "%%~nxD"=="build" (
        pushd "%%~D"
        for /r %%v in (*.h) do set CPP_FILES=%CPP_FILES% "%%v"
        for /r %%v in (*.cc) do set CPP_FILES=%CPP_FILES% "%%v"
        popd
    )
)

rem Run cmake-format
echo %GREEN%Formatting cmake files...%NO_COLOR%
cmake-format %CMAKE_FILES% --in-place

rem Run cmake-lint
echo %GREEN%Linting cmake files...%NO_COLOR%
cmake-lint %CMAKE_FILES%

rem Run clang-format
echo %GREEN%Formatting C++ files...%NO_COLOR%
clang-format -i %CPP_FILES%

rem Run clang-tidy
echo %GREEN%Linting C++ files...%NO_COLOR%
clang-tidy %CPP_FILES% --^
    -Isystem^
    -I./^
    -I./build/_deps/cxxopts-src/include^
    -I./build/_deps/restbed-src/source^
    -I./build/_deps/json-src/include^
    -I./build/_deps/cpr-src/include^
    -I./build/_deps/cpr-build/cpr_generated_includes^
    -I./build/_deps/curl-src/include^
    -I./build/_deps/jsonvalidator-src/src^
    -I./build/_deps/valijson-src/include^
    -I./build/_deps/blst-src/bindings
