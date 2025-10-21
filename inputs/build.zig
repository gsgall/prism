const std = @import("std");
const zcc = @import("compile_commands");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const linkage = b.option(std.builtin.LinkMode, "linkage", "static or dynamic linkage") orelse .static;
    const build_main = b.option(bool, "main", "whether or not to build the main executable") orelse false;
    var targets = std.ArrayListUnmanaged(*std.Build.Step.Compile){};

    // All other libraries link against YOUR cpp_runtime
    const yaml_cpp = b.dependency("yaml-cpp", .{
        .target = target,
        .optimize = optimize,
    });
    const lib_yaml_cpp = yaml_cpp.artifact("yaml-cpp");
    // Build and install the actual library
    const lib_inputs = b.addLibrary(.{
        .name = "inputs",
        .linkage = linkage,
        .root_module = b.createModule(.{ .target = target, .optimize = optimize }),
    });

    lib_inputs.addIncludePath(b.path("include/inputs"));
    lib_inputs.addIncludePath(b.path("include"));
    lib_inputs.installHeadersDirectory(b.path("include"), "", .{});
    lib_inputs.addCSourceFiles(.{
        .root = b.path("src"),
        .files = &.{
            //
            "TypeNameHelper.C",
            "InputErrorHelper.C",
            "Parameter.C",
            "InputParameters.C",
        },
        .flags = &.{ "-std=c++17", "-g", "-fPIC" },
    });
    lib_inputs.linkLibrary(lib_yaml_cpp);
    lib_inputs.linkLibCpp();

    if (build_main) {
        const exe = b.addExecutable(.{ .name = "main", .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
        }) });

        exe.linkLibrary(lib_inputs);
        exe.linkLibrary(lib_yaml_cpp);
        exe.addCSourceFiles(.{
            .files = &.{
                "main.C",
            },
            .flags = &.{
                "-std=c++17",
                "-g",
            },
        });
        exe.linkLibCpp();
        b.installArtifact(exe);
        targets.append(b.allocator, exe) catch @panic("OOM");
    }

    b.installArtifact(lib_inputs);
    targets.append(b.allocator, lib_inputs) catch @panic("OOM");
    b.installArtifact(lib_yaml_cpp);
    targets.append(b.allocator, lib_yaml_cpp) catch @panic("OOM");
    _ = zcc.createStep(b, "cdb", targets.toOwnedSlice(b.allocator) catch @panic("OOM"));
}
