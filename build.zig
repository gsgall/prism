const std = @import("std");
const zcc = @import("compile_commands");
pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const linkage = b.option(std.builtin.LinkMode, "linkage", "static or dynamic linkage") orelse .static;
    const build_main = b.option(bool, "main", "whether or not to build the main executable") orelse false;
    var targets = std.ArrayListUnmanaged(*std.Build.Step.Compile){};

    const inputs = b.dependency("inputs", .{ .target = target, .optimize = optimize });
    const lib_inputs = inputs.artifact("inputs");
    const lib_yaml_cpp = inputs.artifact("yaml-cpp");

    const lib_prism = b.addLibrary(.{
        .name = "prism",
        .linkage = linkage,
        .root_module = b.createModule(.{ .target = target, .optimize = optimize }),
    });

    lib_prism.addIncludePath(b.path("include/prism/core"));
    lib_prism.addIncludePath(b.path("include/prism/reactions"));
    lib_prism.addIncludePath(inputs.path("include"));
    lib_prism.installHeadersDirectory(b.path("include"), "", .{});
    lib_prism.addCSourceFiles(.{
        .root = b.path("src/core"),
        .files = &.{
            "NetworkParser.C",
            "PrismErrorHelper.C",
            "PrismTypes.C",
            "ReactionManager.C",
            "ReactionRegistrar.C",
            "Species.C",
            "SpeciesManager.C",
            "StringHelper.C",
        },
        .flags = &.{ "-std=c++17", "-g", "-fPIC" },
    });
    lib_prism.addCSourceFiles(.{
        .root = b.path("src/reactions"),
        .files = &.{
            //
            "ArrheniusRateReaction.C",
            "ConstantRateReaction.C",
            "RateReactionBase.C",
            "ReactionBase.C",
            "XSecReactionBase.C",
        },
        .flags = &.{ "-std=c++17", "-g", "-fPIC" },
    });

    lib_prism.linkLibrary(lib_yaml_cpp);
    lib_prism.linkLibrary(lib_inputs);
    lib_prism.linkLibCpp();

    if (build_main) {
        const exe = b.addExecutable(.{ .name = "main", .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
        }) });
        exe.addIncludePath(inputs.path("include"));
        exe.linkLibrary(lib_inputs);
        exe.linkLibrary(lib_yaml_cpp);
        exe.linkLibrary(lib_prism);
        exe.linkLibCpp();
        exe.addCSourceFile(.{
            .file = b.path("main.C"),
            .flags = &.{
                "-std=c++17",
                "-g",
                "-Wall",
                "-Wextra",
            },
        });
        b.installArtifact(exe);
        targets.append(b.allocator, exe) catch @panic("OOM");
    }
    b.installArtifact(lib_prism);
    targets.append(b.allocator, lib_prism) catch @panic("OOM");
    b.installArtifact(lib_inputs);
    targets.append(b.allocator, lib_inputs) catch @panic("OOM");
    b.installArtifact(lib_yaml_cpp);
    targets.append(b.allocator, lib_yaml_cpp) catch @panic("OOM");
    _ = zcc.createStep(b, "cdb", targets.toOwnedSlice(b.allocator) catch @panic("OOM"));
}
