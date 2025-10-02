from clang_tidy_config import global_tidy_options_string

global_makefile = {
    'CXX': 'clang++',
    'CXXFLAGS': '-x c++ -g -O3 -Wall -pedantic -pipe -std=c++20 -DGLM_FORCE_SWIZZLE -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE -I ./app -I ./glm -I./glad/include',
    'LDFLAGS': '-g -O3 -Wall -pedantic -pipe -std=c++20',
    # Linting & Formatting tests
    'do_format_check': True,
    'do_lint_check': True,
    # clang-tidy options, can be overridden per part
    'tidy_opts': global_tidy_options_string,
    'skip_compile_cmd': False,
    # Google Test & Google Mock
    'do_unit_tests': False,
    'gtest_dependencies': '$(TARGET)_functions.o $(TARGET)_unittest.cc',
    # pylint: disable-next=line-too-long
    'gtest_compile_cmd': '@$(CXX) $(CXXFLAGS) $(GTESTINCLUDE) $(LDFLAGS) -o unittest $(TARGET)_unittest.cc $(TARGET)_functions.cc $(GTESTLIBS)',
    'gtest_run': '@./unittest --gtest_output=$(GTEST_OUTPUT_FORMAT):$(GTEST_OUTPUT_FILE)',
    'GTEST_OUTPUT_FORMAT': 'json',
    'GTEST_OUTPUT_FILE': 'test_detail.json',
    # Doxygen
    'DOXYGEN': 'doxygen',
    'DOCDIR': 'doc',
    'tidyopts': global_tidy_options_string,
    # Linux specific settings
    # pylint: disable-next=line-too-long
    'linux_CXXFLAGS': '-D LINUX',
    'linux_LDFLAGS': '-lGL -lglfw',
    'linux_sed': 'sed',
    # pylint: disable-next=line-too-long
    'linux_GTESTINCLUDE': '-D LINUX',
    'linux_GTESTLIBS': '-L /usr/lib/gcc/x86_64-linux-gnu/11 -lgtest -lgtest_main -lpthread',
    'linux_GLAD': '/usr/bin/glad',
    'linux_GLADFLAGS': '--api gl:core=3.3 --out-path glad c --debug --header-only',
    # Darwin (macOS) specific settings assuming MacPorts, not Homebrew
    'darwin_CXXFLAGS': '-D OSX -I/opt/local/include',
    # pylint: disable-next=line-too-long
    'darwin_LDFLAGS': '-L/opt/local/lib -lglfw',
    'darwin_sed': 'gsed',
    'darwin_GTESTINCLUDE': '-I /opt/local/include -I /opt/local/src/googletest',
    'darwin_GTESTLIBS': '-L /opt/local/libexec/llvm-18/lib/libc++ -L /opt/local/libexec/llvm-18/lib -L /opt/local/libexec/llvm-18/lib/libunwind -L /opt/local/lib -lgtest -lgtest_main -lunwind -rpath /opt/local/libexec/llvm-18/lib/libc++ -rpath /opt/local/libexec/llvm-18/lib/libunwind',
    'darwin_GLAD': 'glad-3.13',
    'darwin_GLADFLAGS': '--api gl:core=3.3 --out-path glad c --debug --header-only',
}
