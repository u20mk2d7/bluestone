from conan import ConanFile
from conan.tools.cmake import cmake_layout

class Bluestone(ConanFile):
    name = "bluestone"
    version = "1.0.0"
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    
    # This generates the conan_toolchain.cmake and target files for find_package
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        # --- Core Performance & Formats ---
        self.requires("simdjson/4.6.3")
        self.requires("flatbuffers/25.9.23")
        
        # --- Logic & Utilities ---
        self.requires("boost/1.91.0", options={"without_cobalt": True})
        self.requires("abseil/20260107.1")
        
        self.requires("protobuf/7.35.0")

        # --- Math & Science ---
        self.requires("eigen/5.0.1")
        
        # --- Security, Database & Network ---
        self.requires("openssl/3.6.2")
        
        # --- Testing & Benchmarking ---
        self.requires("gtest/1.17.0")
        self.requires("benchmark/1.9.5")

        #self.requires("zeromq/4.3.5")

    def layout(self):
        # This sets up the standard folder structure for your build
        cmake_layout(self)

