const std = @import("std");
const zcc = @import("compile_commands");
pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const linkage = b.option(std.builtin.LinkMode, "linkage", "static or dynamic linkage") orelse .static;
    var targets = std.ArrayListUnmanaged(*std.Build.Step.Compile){};

    const googletest_dep = b.dependency("googletest", .{});

    const gtest = b.addLibrary(.{
        .name = "gtest",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
        }),
        .linkage = linkage,
    });
    gtest.linkLibC();
    gtest.linkLibCpp();
    gtest.addCSourceFile(.{
        .file = googletest_dep.path("googletest/src/gtest-all.cc"),
        .flags = &.{},
    });
    gtest.addIncludePath(googletest_dep.path("googletest/include"));
    gtest.addIncludePath(googletest_dep.path("googletest"));
    gtest.installHeadersDirectory(googletest_dep.path("googletest/include"), ".", .{});

    targets.append(b.allocator, gtest) catch @panic("OOM");
    b.installArtifact(gtest);

    const gtest_main = b.addLibrary(.{
        .name = "gtest_main",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
        }),
        .linkage = linkage,
    });
    gtest_main.linkLibC();
    gtest_main.linkLibCpp();
    gtest_main.addCSourceFile(.{
        .file = googletest_dep.path("googletest/src/gtest_main.cc"),
        .flags = &.{},
    });
    gtest_main.addIncludePath(googletest_dep.path("googletest/include"));
    gtest_main.addIncludePath(googletest_dep.path("googletest"));
    gtest_main.installHeadersDirectory(googletest_dep.path("googletest/include"), ".", .{});

    targets.append(b.allocator, gtest_main) catch @panic("OOM");
    b.installArtifact(gtest_main);

    const inputs = b.dependency("inputs", .{
        .target = target,
        .optimize = optimize,
    });
    const lib_inputs = inputs.artifact("inputs");
    targets.append(b.allocator, lib_inputs) catch @panic("OOM");
    b.installArtifact(lib_inputs);

    const lib_yaml_cpp = inputs.artifact("yaml-cpp");
    targets.append(b.allocator, lib_yaml_cpp) catch @panic("OOM");
    b.installArtifact(lib_yaml_cpp);

    const prism = b.dependency("prism", .{
        .target = target,
        .optimize = optimize,
    });
    const lib_prism = prism.artifact("prism");
    targets.append(b.allocator, lib_prism) catch @panic("OOM");
    b.installArtifact(lib_prism);

    const exe = b.addExecutable(.{ .name = "run_tests", .root_module = b.createModule(.{
        .target = target,
        .optimize = optimize,
    }) });

    exe.root_module.addCMacro("YAML_CPP_STATIC_DEFINE", "");
    exe.addIncludePath(prism.path("inputs/include"));
    exe.linkLibrary(lib_prism);
    exe.linkLibrary(lib_yaml_cpp);
    exe.linkLibrary(lib_inputs);
    exe.linkLibrary(gtest);
    exe.linkLibrary(gtest_main);
    exe.addCSourceFiles(.{
        .root = b.path("tests"),
        .files = &.{
            "NetworkParserTest.C",
            "ReactionManagerTest.C",
            "SpeciesManagerTest.C",
            "StringHelperTest.C",
        },
        .flags = &.{
            "-std=c++17",
            "-g",
        },
    });
    exe.linkLibCpp();
    b.installArtifact(exe);
    targets.append(b.allocator, exe) catch @panic("OOM");
    _ = zcc.createStep(b, "cdb", targets.toOwnedSlice(b.allocator) catch @panic("OOM"));

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the main executable");
    run_step.dependOn(&run_cmd.step);
}
