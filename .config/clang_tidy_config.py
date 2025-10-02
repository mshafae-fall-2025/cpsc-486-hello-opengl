from os import environ
#List of clang tidy checks : https:  // clang.llvm.org/extra/clang-tidy/checks/list.html
global_tidy_checks = (
    '-checks="-*,'
    'boost-*,'
    'bugprone-*,'
    '-bugprone-easily-swappable-parameters,'
    '-bugprone-narrowing-conversions,'
    'clang-analyzer-*,'
    'cppcoreguidelines-*,'
    '-cppcoreguidelines-avoid-do-while,'
    '-cppcoreguidelines-avoid-magic-numbers,'
    '-cppcoreguidelines-narrowing-conversions,'
    '-cppcoreguidelines-pro-type-union-access,'
    '-cppcoreguidelines-owning-memory,'
    'google-*,'
    '-google-build-using-namespace,'
    'llvm-*,'
    '-llvm-else-after-return,'
    '-llvm-header-guard,'
    'misc-*,'
    '-misc-no-recursion,'
    '-misc-unused-parameters,'
    '-misc-include-cleaner,'
    'hicpp-*,'
    '-hicpp-special-member-functions,'
    'modernize-*,'
    '-modernize-use-trailing-return-type,'
    '-modernize-use-nodiscard,'
    '-modernize-pass-by-value,'
    'performance-*,'
    '-performance-unnecessary-value-param,'
    'portability-*,'
    'readability-*,'
    '-readability-magic-numbers,'
    '-readability-else-after-return,'
    '-readability-simplify-boolean-expr,'
    '-readability-identifier-length,'
    '"'
)

if environ.get('LABSOLUTION', None):
    index = global_tidy_checks.rfind(',') + 1
    global_tidy_checks = (
        global_tidy_checks[:index]
        + '-google-readability-todo,'
        + global_tidy_checks[index:]
    )


global_tidy_config = (
    '-config="{CheckOptions: ['
        '{key: readability-identifier-naming.ClassCase, value: CamelCase}, '
        '{key: readability-identifier-naming.MemberCase, value: lower_case}, '
        '{key: readability-identifier-naming.ClassMemberCase, value: lower_case}, '
        '{key: readability-identifier-naming.PrivateMemberCase, value: lower_case}, '
        '{key: readability-identifier-naming.PublicMemberCase, value: lower_case}, '
        '{key: readability-identifier-naming.ProtectedMemberCase, value: lower_case}, '
        '{key: readability-identifier-naming.MethodCase, value: CamelCase}, '
        '{key: readability-identifier-naming.PrivateMemberSuffix, value: _}, '
        '{key: readability-identifier-naming.PublicMemberSuffix, value: \'\'}, '
        '{key: readability-identifier-naming.ConstexprVariableCase, value: CamelCase}, '
        '{key: readability-identifier-naming.ConstexprVariablePrefix, value: k}, '
        '{key: readability-identifier-naming.EnumCase, value: CamelCase}, '
        '{key: readability-identifier-naming.EnumConstantCase, value: CamelCase}, '
        '{key: readability-identifier-naming.EnumConstantPrefix, value: k}, '
        '{key: readability-identifier-naming.FunctionCase, value: CamelCase}, '
        '{key: readability-identifier-naming.GlobalConstantCase, value: CamelCase}, '
        '{key: readability-identifier-naming.GlobalConstantPrefix, value: k}, '
        '{key: readability-identifier-naming.StaticConstantCase, value: CamelCase}, '
        '{key: readability-identifier-naming.StaticConstantPrefix, value: k}, '
        '{key: readability-identifier-naming.StaticVariableCase, value: lower_case}, '
        '{key: readability-identifier-naming.MacroDefinitionCase, value: UPPER_CASE}, '
        '{key: readability-identifier-naming.MacroDefinitionIgnoredRegexp, value: \'^[A-Z]+(_[A-Z]+)*_$\'}, '
        '{key: readability-identifier-naming.NamespaceCase, value: lower_case}, '
        '{key: readability-identifier-naming.ParameterCase, value: lower_case}, '
        '{key: readability-identifier-naming.TypeAliasCase, value: CamelCase}, '
        '{key: readability-identifier-naming.TypedefCase, value: CamelCase}, '
        '{key: readability-identifier-naming.VariableCase, value: lower_case}, '
        '{key: readability-identifier-naming.IgnoreMainLikeFunctions, value: 1}, '
        '{key: cppcoreguidelines-special-member-functions.AllowMissingMoveFunctionsWhenCopyIsDeleted, value: true}, '
    ']}"'
)

tidy_compiler_options = (
    '-std=c++20 -I /opt/local/include -I /usr/local/include '
)

#Concatenate clang's checks and options into a single string ready to
#be used as a command line option.
global_tidy_options_string = f'{global_tidy_checks} {global_tidy_config}'
#global_tidy_options_string = f '{global_tidy_checks}'
#global_tidy_options_string = f '{global_tidy_config}'


def lint_check(file, tidy_options, compiler_options, cmd='clang-tidy'):
    """ Use clang-tidy to lint the file. Options for clang-tidy \
    defined in the function. """
    import subprocess
    _cmd = f'{cmd} {tidy_options} {file} -- {compiler_options}'
    proc = subprocess.run(
        [_cmd],
        capture_output=True,
        shell=True,
        timeout=60,
        check=False,
        text=True,
    )
    if proc.returncode != 0:
        print(f'command:\n{_cmd}')
        print(str(proc.stderr))
        raise ChildProcessError('Error linting.')
    linter_warnings = str(proc.stdout).split('\n')
    linter_warnings = [line for line in linter_warnings if line != '']
    return linter_warnings

def main():
    global global_tidy_options_string
    global tidy_compiler_options
    if len(sys.argv) < 2:
        print('Please provide a list of files to lint.')
        print(f'ex. {sys.argv[0]} file1.cc file2.cc file1.h')
        return 1
    for file in sys.argv[1:]:
        try:
            linter_warnings = lint_check(file, global_tidy_options_string, tidy_compiler_options)
        except ChildProcessError as exception:
            print(exception)
            return 1
        if len (linter_warnings) > 0:
            warning_string = '\n'.join(linter_warnings)
        else:
            warning_string = 'No lint'
        print(f'{file}: {warning_string}')
    return 0

if __name__ == '__main__' :
    import sys
    sys.exit(main())
