#!/usr/bin/env python3

"""A lab configuration file."""

from datetime import date
from os import environ
from makefile_config import global_makefile

#
# -- START -- For each lab update this section -- START --
#

# A list of target names in the order of the lab's parts.
targets = 'blink'.split()

lab = {
    # Due date for labs
    'mon_duedate': date(2023, 5, 8),
    'tues_duedate': date(2024, 5, 1),
    'wed_duedate': date(2024, 5, 1),
    'num_parts': len(targets),
    # Assume no part-n directories, it's a flat project with one part
    'single_project': True,
    # Makefile name, in a rare situation when you don't want 'Makefile'
    'makefile_name': 'Makefile',
    # Prefix Makefiles with a period to hid them
    'hidden_makefiles': False,
    # Where the students' authorship information is stored, path relative to
    # root of repository.
    'author_file': 'AUTHORS.txt',
    # Configuration of target, source files, and header files for each part. These are the files
    # that will be checked for headers, format, and lint.
    # other_src and other_header are files that are needed for building and are not graded/assessed.
    # Unit tests are always built as:
    # $(CXX) $(GTESTINCLUDE) $(LDFLAGS) -o unittest $(TARGET)_unittest.cc $(TARGET)_functions.o
    # where the unit tests are in $(TARGET)_unittest.cc with the
    # sole dependency on $(TARGET)_functions.o
    'parts': [
        {
            'target': f'{targets[0]}',
            'src': 'main.cc',
            'header': 'blink_scene.h',
            'other_src': 'app/gl.cc app/glfwapp.cc app/glslshader.cc app/msutil.cc',
            'other_header': 'app/glfwapp.h app/glslshader.h app/hid.h app/msutil.h app/scene.h ',
            'test_main': 'run_p1',
        },
    ],
}

#
# -- END -- For each lab update this section -- END --
#

if lab['single_project']:
    assert(len(targets) == 1)

makefiles = [global_makefile.copy() for i in range(len(targets))]

# Specific Makefile settings for each lab part.
# If a lab has unit tests, set 'do_unit_tests' to True
makefiles[0].update(
    {
        'do_unit_tests': False,
        'do_format_check': True,
        'do_lint_check': True,
    }
)

# Merge the makefile dict into the lab_dict's part's dictionary
for lab_dict, makefile in zip(lab['parts'], makefiles):
    lab_dict.update(makefile)

def main():
    """ A function to access any value within the lab's configuration given a series of keys for use with scripts and workflows. Returns 0 if executed without a problem. """
    import sys
    if len(sys.argv) > 1:
        primary_key = sys.argv[1]
        if primary_key == 'gradedsrc':
            allsrc = ''
            for p in lab['parts']:
                allsrc += '{} {} '.format(p['src'], p['header'])
            value = allsrc
        elif primary_key == 'makefile_name':
            if lab['hidden_makefiles']:
                value = '.{}'.format(lab['makefile_name'])
            else:
                value = lab['makefile_name']
        elif primary_key == 'parts' and len(sys.argv) >= 4:
            try:
                part_number = int(sys.argv[2])
            except ValueError as exception:
                sys.exit(1)
            sub_key = sys.argv[3]
            try:
                parts_list = lab[primary_key]
            except KeyError as exception:
                sys.exit(1)
            try:
                part = parts_list[part_number]
            except IndexError as exception:
                sys.exit(1)
            try:
                value = part[sub_key]
            except KeyError as exception:
                sys.exit(1)
        else:
            try:
                value = lab[primary_key]
            except KeyError as exception:
                sys.exit(1)
    print(value)
    sys.exit(0)


if __name__ == '__main__':
    main()
