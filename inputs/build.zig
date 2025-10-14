const std = @import("std");
const zcc = @import("compile_commands");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const exe = b.addExecutable(.{ .name = "main", .root_module = b.createModule(.{
        .target = target,
        .optimize = optimize,
    }) });

    exe.addIncludePath(b.path("include"));
    exe.addIncludePath(b.path("include/inputs"));
    exe.addIncludePath(b.path("yaml-cpp-0.8.0/include"));
    exe.addCSourceFiles(.{
        .files = &.{
            "main.C",
            "src/InputErrorHelper.C",
            "src/InputParameters.C",
            "src/TypeNameHelper.C",
            "src/Parameter.C",
            "yaml-cpp-0.8.0/src/binary.cpp",
            "yaml-cpp-0.8.0/src/convert.cpp",
            "yaml-cpp-0.8.0/src/depthguard.cpp",
            "yaml-cpp-0.8.0/src/directives.cpp",
            "yaml-cpp-0.8.0/src/emit.cpp",
            "yaml-cpp-0.8.0/src/emitfromevents.cpp",
            "yaml-cpp-0.8.0/src/emitter.cpp",
            "yaml-cpp-0.8.0/src/emitterstate.cpp",
            "yaml-cpp-0.8.0/src/emitterutils.cpp",
            "yaml-cpp-0.8.0/src/exceptions.cpp",
            "yaml-cpp-0.8.0/src/exp.cpp",
            "yaml-cpp-0.8.0/src/memory.cpp",
            "yaml-cpp-0.8.0/src/node_data.cpp",
            "yaml-cpp-0.8.0/src/node.cpp",
            "yaml-cpp-0.8.0/src/nodebuilder.cpp",
            "yaml-cpp-0.8.0/src/nodeevents.cpp",
            "yaml-cpp-0.8.0/src/null.cpp",
            "yaml-cpp-0.8.0/src/ostream_wrapper.cpp",
            "yaml-cpp-0.8.0/src/parse.cpp",
            "yaml-cpp-0.8.0/src/parser.cpp",
            "yaml-cpp-0.8.0/src/regex_yaml.cpp",
            "yaml-cpp-0.8.0/src/scanner.cpp",
            "yaml-cpp-0.8.0/src/scanscalar.cpp",
            "yaml-cpp-0.8.0/src/scantag.cpp",
            "yaml-cpp-0.8.0/src/scantoken.cpp",
            "yaml-cpp-0.8.0/src/simplekey.cpp",
            "yaml-cpp-0.8.0/src/singledocparser.cpp",
            "yaml-cpp-0.8.0/src/stream.cpp",
            "yaml-cpp-0.8.0/src/tag.cpp",
        },
        .flags = &.{
            "-std=c++17",
            "-g",
        },
    });
    exe.linkLibCpp();
    b.installArtifact(exe);
}
