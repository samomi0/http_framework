import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake

class HttpFramework(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("boost/1.83.0")
        self.requires("spdlog/1.12.0")
        self.requires("fmt/10.2.1")

    def generate(self):
        tc = CMakeToolchain(self)
        # tc.cache_variables["BUILD_DEMO"] = False
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def layout(self):
        self.folders.source = "."
        self.folders.build = os.path.join("build", str(self.settings.build_type))
        self.folders.generators = os.path.join(self.folders.build, "generators")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()