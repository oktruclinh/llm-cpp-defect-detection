#!/usr/bin/env python3
"""Per-project build recipes for Tier B/C (project-build) defects.

Unlike Tier A (header-only, compiled with `g++ -I include`), these projects must be
BUILT, and the generated gtest linked against the built artifacts. The portable
mechanism is `compile_commands.json`: we build the project once (exporting the
compile DB), then reuse the exact include dirs / defines / std of the focal
translation unit to compile the test, and link against the recipe's libs.

⚠️ UNVERIFIED ON A BUILD MACHINE. Every `build_cmd` / `link_libs` below is a
best-effort starting point to be confirmed during the B0 box spike (one project
end-to-end first). Treat TODOs as required checks, not optional.
"""
from dataclasses import dataclass, field


@dataclass
class ProjectRecipe:
    mode: str = "project_build"            # vs "header_only" (Tier A, in cpp_harness)
    std: str = "c++17"
    # shell command(s) to configure+build at the CURRENT checkout, run with cwd=repo.
    # MUST export compile_commands.json (CMake: -DCMAKE_EXPORT_COMPILE_COMMANDS=ON;
    # Make: wrap with `bear --`). MUST build the library/objects the focal TU lands in.
    build_cmd: str = ""
    # where compile_commands.json ends up, relative to repo root
    compile_db: str = "build/compile_commands.json"
    # libraries / link flags to link the test against (the built focal lib + deps)
    link_libs: list = field(default_factory=list)
    # extra include dirs (relative to repo) beyond what compile_commands gives
    extra_includes: list = field(default_factory=list)
    # incremental rebuild command after swapping the focal file to buggy (cwd=repo)
    rebuild_cmd: str = ""                   # default: same as build_cmd (incremental)
    notes: str = ""


# All builds use clang-16 + libc++ (Defects4C's toolchain, apt-installed) so old-commit
# code compiles and the ABI matches the libc++ gtest + test. See tierbc_harness CXX_BC.
_CLANG = "CC=clang-16 CXX=clang++-16"
_CXXF = "-DCMAKE_CXX_FLAGS='-stdlib=libc++ -w'"

# Tier B candidates — START the box spike with arrow (uses gtest) or nng (small).
RECIPES_BC = {
    # danmar/cppcheck (Tier C, 31 defects) — VALIDATED on box: build the lib OBJECT
    # target only (the CLI has a modern-compiler error we don't need), then ar into a .a.
    "danmar___cppcheck": ProjectRecipe(
        std="c++17",
        build_cmd=(f"{_CLANG} cmake -S . -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "
                   f"-DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=OFF -DHAVE_RULES=OFF "
                   f"-DWARNINGS_ARE_ERRORS=OFF {_CXXF} && "
                   "cmake --build build --target lib_objs -j4 && "
                   "ar rcs build/libcppcheck.a build/lib/CMakeFiles/lib_objs.dir/*.o "
        "build/externals/*/CMakeFiles/*_objs.dir/*.o"),
        compile_db="build/compile_commands.json",
        link_libs=["build/libcppcheck.a"],
        rebuild_cmd=("cmake --build build --target lib_objs -j4 && "
                     "ar rcs build/libcppcheck.a build/lib/CMakeFiles/lib_objs.dir/*.o "
        "build/externals/*/CMakeFiles/*_objs.dir/*.o"),
        notes="lib_objs object target only; focal funcs are in lib/*.cpp; needs clang-16+libc++."),
    # apache/arrow — large CMake project, already uses GoogleTest. Heavy build.
    "apache___arrow": ProjectRecipe(
        std="c++17",
        # MINIMAL core libarrow (everything optional OFF) so table_builder.cc /
        # concatenate.cc / compare.cc compile+link with almost no deps. clang-16/libc++
        # to match the gtest ABI (was missing before -> would have linked the wrong stdlib).
        build_cmd=(f"{_CLANG} cmake -S cpp -B cpp/build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "
                   "-DCMAKE_BUILD_TYPE=Debug -DARROW_BUILD_STATIC=ON -DARROW_BUILD_SHARED=OFF "
                   "-DARROW_BUILD_TESTS=OFF -DARROW_BUILD_BENCHMARKS=OFF -DARROW_BUILD_EXAMPLES=OFF "
                   "-DARROW_BUILD_UTILITIES=OFF -DARROW_DATASET=OFF -DARROW_COMPUTE=OFF "
                   "-DARROW_CSV=OFF -DARROW_JSON=OFF -DARROW_IPC=OFF -DARROW_FILESYSTEM=OFF "
                   "-DARROW_JEMALLOC=OFF -DARROW_MIMALLOC=OFF -DARROW_USE_GLOG=OFF "
                   "-DARROW_WITH_RE2=OFF -DARROW_WITH_UTF8PROC=OFF -DARROW_WITH_BROTLI=OFF "
                   "-DARROW_WITH_SNAPPY=OFF -DARROW_WITH_ZLIB=OFF -DARROW_WITH_ZSTD=OFF "
                   "-DARROW_WITH_LZ4=OFF -DARROW_WITH_BZ2=OFF -DARROW_DEPENDENCY_SOURCE=BUNDLED "
                   f"{_CXXF} && cmake --build cpp/build --target arrow -j4"),
        compile_db="cpp/build/compile_commands.json",
        link_libs=["cpp/build/debug/libarrow.a", "-lpthread"],   # TODO box: confirm debug/ subdir + any bundled .a
        extra_includes=["cpp/src", "cpp/build/src"],             # generated arrow/util/config.h lives under build/src
        rebuild_cmd=f"{_CLANG} cmake --build cpp/build --target arrow -j4",
        notes="UNVERIFIED — run campaign_arrow_cc.py with ARROW_N=1 (table_builder) FIRST to validate "
              "build+link before the rest. arrow build is LONG (~20-40min); switching defect commits forces "
              "near-full rebuilds, so do table_builder alone unless time allows. If link fails: check "
              "cpp/build/debug/ for libarrow.a and add any bundled dep .a it needs. Snapshot cpp/build after success."),
    # nanomsg/nng — small CMake C library; focal is C, gtest links the C lib.
    "nanomsg___nng": ProjectRecipe(
        std="c++17",
        build_cmd=("cmake -S . -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "
                   "-DNNG_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug && cmake --build build --target nng"),
        compile_db="build/compile_commands.json",
        link_libs=["build/libnng.a", "-lpthread"],               # TODO verify
        rebuild_cmd="cmake --build build --target nng",
        notes="C project — extern \"C\" the focal decls in the test if needed."),
    # facebook/rocksdb — CMake, uses gtest internally.
    "facebook___rocksdb": ProjectRecipe(
        build_cmd=("cmake -S . -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "
                   "-DWITH_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug && cmake --build build --target rocksdb"),
        compile_db="build/compile_commands.json",
        link_libs=["build/librocksdb.a", "-lpthread", "-ldl"],   # TODO verify
        rebuild_cmd="cmake --build build --target rocksdb",
        notes="Large; memory-heavy build."),
    # KhronosGroup/SPIRV-Tools — CMake; focal in source/opt/ (optimizer passes, internal).
    # Deps fetched PER-CHECKOUT via git-sync-deps (spirv-headers, abseil, re2, effcee, gtest)
    # so versions match the old commit. Heavy (abseil); validate ONE defect before batch.
    "KhronosGroup___SPIRV-Tools": ProjectRecipe(
        std="c++17",
        # `git checkout <defect>` reverts our Py3.12 fix, so re-apply the rU->r patch first
        # (old utils/update_build_version.py uses open(...,'rU'), removed in Python 3.11).
        build_cmd=("sed -i \"s/'rU'/'r'/g\" utils/update_build_version.py && "
                   "python3 utils/git-sync-deps && "
                   f"{_CLANG} cmake -S . -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "
                   f"-DSPIRV_SKIP_TESTS=ON -DSPIRV_WERROR=OFF -DEFFCEE_BUILD_TESTING=OFF "
                   f"-DCMAKE_BUILD_TYPE=Debug {_CXXF} && "
                   "cmake --build build --target SPIRV-Tools-opt -j4"),
        compile_db="build/compile_commands.json",
        # POSITIVE CONTROL PASSED (2026-06-24): a public-API test (spvtools::Optimizer) links
        # with just these two .a (no abseil/re2/effcee) — so a test non-compile here is a
        # GENUINE unreachability finding, not a build artifact.
        link_libs=["build/source/opt/libSPIRV-Tools-opt.a", "build/source/libSPIRV-Tools.a"],
        extra_includes=["include", "build"],
        rebuild_cmd=("sed -i \"s/'rU'/'r'/g\" utils/update_build_version.py && "
                     "cmake --build build --target SPIRV-Tools-opt -j4"),
        notes="Py3.12 rU patch per checkout; opt+core .a suffice; 2019-era commits need no abseil."),
}
