from conans import ConanFile, CMake, tools


class GsfConan(ConanFile):
    name = "benchmarks"
    version = "0.0.1"
    license = "MIT"
    author = "liumj mr.liumingjie@gmail.com"
    description = "simple test and benchmark starter"
    topics = ("starter", "benchmark", "test")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": True,
        "fPIC": True,
    }
    generators = "cmake"
    # exports_sources = "src/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("fmt/10.1.0")
        self.requires("bshoshany-thread-pool/3.5.0")
        self.requires("date/3.0.1")
        self.requires("catch2/2.13.9")
        self.requires("parallel-hashmap/1.37")
        self.requires("tl-expected/20190710")

    # def imports(self):
        # self.copy("*.so", dst="lib", src="lib", root_package="gsf2*")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    # def package(self):
    #     self.copy("*.h", dst="include/gsf2/utils", src="src", keep_path=False)
    #     self.copy("*.hpp", dst="include/gsf2/utils", src="src", keep_path=True)
    #     self.copy("*.lib", dst="lib", keep_path=False)
    #     self.copy("*.dll", dst="bin", keep_path=False)
    #     self.copy("*.so", dst="lib", keep_path=False)
    #     self.copy("*.dylib", dst="lib", keep_path=False)
    #     self.copy("*.a", dst="lib", keep_path=False)

    # def package_info(self):
    #     self.cpp_info.libs = ["gsf2-utils"]