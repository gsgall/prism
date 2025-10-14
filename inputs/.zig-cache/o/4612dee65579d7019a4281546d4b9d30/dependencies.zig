pub const packages = struct {
    pub const @"yaml_cpp-0.16.0-dev.368+2a97e0af6-4nceVTbzSgCCQPhCQH7j5yy-IgoTvWA7zukGMGUNSLWQ" = struct {
        pub const build_root = "/Users/graysongall/.cache/zig/p/yaml_cpp-0.16.0-dev.368+2a97e0af6-4nceVTbzSgCCQPhCQH7j5yy-IgoTvWA7zukGMGUNSLWQ";
        pub const build_zig = @import("yaml_cpp-0.16.0-dev.368+2a97e0af6-4nceVTbzSgCCQPhCQH7j5yy-IgoTvWA7zukGMGUNSLWQ");
        pub const deps: []const struct { []const u8, []const u8 } = &.{
            .{ "compile_commands", "zig_compile_commands-0.0.1-OZg5-ULBAABTh3NXO3WXoSUX1474ez0EouuoT2yDANhz" },
        };
    };
    pub const @"zig_compile_commands-0.0.1-OZg5-ULBAABTh3NXO3WXoSUX1474ez0EouuoT2yDANhz" = struct {
        pub const build_root = "/Users/graysongall/.cache/zig/p/zig_compile_commands-0.0.1-OZg5-ULBAABTh3NXO3WXoSUX1474ez0EouuoT2yDANhz";
        pub const build_zig = @import("zig_compile_commands-0.0.1-OZg5-ULBAABTh3NXO3WXoSUX1474ez0EouuoT2yDANhz");
        pub const deps: []const struct { []const u8, []const u8 } = &.{
        };
    };
};

pub const root_deps: []const struct { []const u8, []const u8 } = &.{
    .{ "compile_commands", "zig_compile_commands-0.0.1-OZg5-ULBAABTh3NXO3WXoSUX1474ez0EouuoT2yDANhz" },
    .{ "yaml_cpp", "yaml_cpp-0.16.0-dev.368+2a97e0af6-4nceVTbzSgCCQPhCQH7j5yy-IgoTvWA7zukGMGUNSLWQ" },
};
